#pragma once

#include <functional>
#include "CallbackBase.hpp"
#include "Configs.h"
#include <thread>

#include "Array.hpp"

using VoidCallback = std::function<void()>;

class ScreenScanner final : public CallbackBase<VoidCallback>
{
public:
    explicit ScreenScanner(const Configuration& config)
    : _config(config)
    {
        const auto vpParams = config.ViewportParameters;
        _data = new Pixel[vpParams.Width * vpParams.Height];
    }

    ~ScreenScanner();

    void Start();
    void Stop();

    void SetNeedScanning(bool flag);

private:
    Configuration _config {};

    bool _scanActive { false };
    bool _needScan { true };
    std::thread* _worker { nullptr };

    mutable Pixel* _data;

    void CaptureScreen() const noexcept;
    bool CheckFramePixels(void) const noexcept;
};

