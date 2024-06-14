#pragma once
#include <unordered_map>

using CbIdType = unsigned long long;

template<typename Callback>
class CallbackBase
{
public:
    CbIdType AddCallback(Callback pointer);
    void RemoveCallback(CbIdType id);
    void EnableCallbacks();
    void DisableCallbacks();

protected:
    void EmitCallback();

private:
    CbIdType _cbId {};
    bool _callbacksEnabled { true };
    std::unordered_map<CbIdType, Callback> _callbacks;
};

template <typename Callback>
CbIdType CallbackBase<Callback>::AddCallback(Callback pointer)
{
    _callbacks.insert_or_assign(_cbId++, pointer);
    return _cbId;
}

template <typename Callback>
void CallbackBase<Callback>::RemoveCallback(CbIdType id)
{
    _callbacks.erase(id);
}

template <typename Callback>
void CallbackBase<Callback>::EnableCallbacks()
{
    _callbacksEnabled = true;
}

template <typename Callback>
void CallbackBase<Callback>::DisableCallbacks()
{
    _callbacksEnabled = false;
}

template <typename Callback>
void CallbackBase<Callback>::EmitCallback()
{
    if(_callbacksEnabled) 
    {
        for (const auto& pair: _callbacks)
        {
            pair.second();
        }
    }
}


