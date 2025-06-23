#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    // シングルトンインスタンスの取得
    static ThreadPool& GetInstance() {
        static ThreadPool instance;
        return instance;
    }

    // スレッドプールの初期化
    void Initialize(size_t numThreads = 0) {
        if (_initialized) return;

        // スレッド数が指定されていない場合はCPUコア数-1を使用
        if (numThreads == 0) {
            numThreads = std::max<size_t>(1, std::thread::hardware_concurrency() - 1);
        }

        for (size_t i = 0; i < numThreads; ++i) {
            _workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(_queueMutex);
                        _condition.wait(lock, [this] {
                            return _stop || !_tasks.empty();
                            });

                        if (_stop && _tasks.empty()) return;

                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }
                    task();
                }
                });
        }

        _initialized = true;
    }

    // タスクの投入
    template<class F, class... Args>
    auto Submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            if (_stop) throw std::runtime_error("ThreadPool is stopped");

            _tasks.emplace([task]() { (*task)(); });
        }
        _condition.notify_one();
        return res;
    }

    // 現在のタスク数の取得
    size_t GetTaskCount() const {
        std::unique_lock<std::mutex> lock(_queueMutex);
        return _tasks.size();
    }

    // スレッド数の取得
    size_t GetThreadCount() const {
        return _workers.size();
    }

    // デストラクタ
    ~ThreadPool() {
        Shutdown();
    }

    // シャットダウン
    void Shutdown() {
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _stop = true;
        }
        _condition.notify_all();
        for (std::thread& worker : _workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        _workers.clear();
        _initialized = false;
        _stop = false;
    }

private:
    // プライベートコンストラクタ（シングルトンパターン）
    ThreadPool() : _stop(false), _initialized(false) {}

    // コピー禁止
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    mutable std::mutex _queueMutex;
    std::condition_variable _condition;
    bool _stop;
    bool _initialized;
};