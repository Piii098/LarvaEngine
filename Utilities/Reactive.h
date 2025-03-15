#pragma once
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <any>

namespace Reactive {

    class Subscription {
    public:
		// �R���X�g���N�^�ōw�ǉ����֐����󂯎��
        Subscription(std::function<void()> func) : _unsubscribeFunc(func) {}
        void Unsubscribe() {
            if (_unsubscribeFunc) {
                _unsubscribeFunc();
                _unsubscribeFunc = nullptr;
            }
        }
        ~Subscription() {
            Unsubscribe();
        }

    private:
        std::function<void()> _unsubscribeFunc;

    };

    template<typename T>
    class Observable {
    public:
        using Callback = std::function<void(const T&)>;

        // �w��
        Subscription Subscribe(Callback onNext) {
            _observers.push_back(onNext);
            auto index = _observers.size() - 1;

            // �w�ǉ����֐���n��
            return Subscription([this, index]() {
                if (index < _observers.size()) {
                    _observers.erase(_observers.begin() + index);
                }
                });
        }

        // �l�̔��s
        void OnNext(const T& value) {
            for (auto& observer : _observers) {
                observer(value);
            }
        }
    private:
        std::vector<std::function<void(const T&)>> _observers;

    };

    class DataStore {
    public:
        template<typename T>
        void SetData(const std::string& key, const T& value) {
            data[key] = value;
            GetOrCreateObservable<T>(key).OnNext(value);
        }

        template<typename T>
        Observable<T>& ObserveData(const std::string& key) {
            return GetOrCreateObservable<T>(key);
        }

        ~DataStore() {
            for (auto& [key, obs] : observables) {
                delete obs; // ���|�C���^�̉��
            }
        }

    private:
        std::unordered_map<std::string, std::any> data;
        std::unordered_map<std::string, void*> observables;

        template<typename T>
        Observable<T>& GetOrCreateObservable(const std::string& key) {
            auto it = observables.find(key);
            if (it == observables.end()) {
                auto* obs = new Observable<T>();
                observables[key] = obs;
                return *obs;
            }
            return *static_cast<Observable<T>*>(it->second);
        }
    };

}