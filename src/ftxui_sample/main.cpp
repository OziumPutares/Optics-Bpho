#include <cassert>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <webgpu/webgpu.h>
namespace {

WGPUAdapter requestAdapterSync(WGPUInstance instance, WGPURequestAdapterOptions const *options)
{
  // A simple structure holding the local information shared with the
  // onAdapterRequestEnded callback.
  WGPUAdapter Adapter = nullptr;
  bool RequestEnded = false;

  auto OnAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                 WGPUAdapter adapter,
                                 WGPUStringView message,
                                 // NOLINTNEXTLINE
                                 void *adpaterData,
                                 void *requestEnded) {
    std::puts("Got adapter");
    // NOLINTNEXTLINE
    WGPUAdapter &r_Adapter = *reinterpret_cast<WGPUAdapter *>(adpaterData);
    // NOLINTNEXTLINE
    bool &REnded = *reinterpret_cast<bool *>(requestEnded);
    if (status == WGPURequestAdapterStatus_Success) {
      r_Adapter = adapter;
    } else {
      std::cout << "Could not get WebGPU adapter: " << message.data << '\n';
    }
    REnded = true;
  };

  // Call to the WebGPU request adapter procedure
  auto Future = wgpuInstanceRequestAdapter(instance,
    options,
    WGPURequestAdapterCallbackInfo{ .nextInChain = nullptr,
      .mode = WGPUCallbackMode_WaitAnyOnly,
      .callback = OnAdapterRequestEnded,
      .userdata1 = static_cast<void *>(&Adapter),
      .userdata2 = static_cast<void *>(&RequestEnded) });
  constexpr int TimeToWait = 0;
  auto FutureResult = WGPUFutureWaitInfo{ .future = Future, .completed = 0U };
  wgpuInstanceWaitAny(instance, 1, &FutureResult, TimeToWait);

  // We wait until userData.requestEnded gets true
#ifdef __EMSCRIPTEN__
  while (!RequestEnded) { emscripten_sleep(100); }
#else
//  while (!RequestEnded) {
//    std::cout << RequestEnded << '\n';
//    constexpr auto TimeToWait = std::chrono::milliseconds(100);
//    std::this_thread::sleep_for(std::chrono::milliseconds(TimeToWait));
//  }
#endif
  std::cout << RequestEnded << '\n';
  assert(RequestEnded);

  return Adapter;
}
void inspectAdapter(WGPUAdapter adapter)
{
#ifndef __EMSCRIPTEN__
  WGPULimits SupportedLimits = {};
  SupportedLimits.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_DAWN
  const bool success = wgpuAdapterGetLimits(adapter, &SupportedLimits) == WGPUStatus_Success;
#else
  const bool Success = wgpuAdapterGetLimits(adapter, &SupportedLimits) == WGPUStatus_Success;
#endif

  if (Success) {
    std::cout << "Adapter limits:" << '\n';
    std::cout << " - maxTextureDimension1D: " << SupportedLimits.maxTextureDimension1D << '\n';
    std::cout << " - maxTextureDimension2D: " << SupportedLimits.maxTextureDimension2D << '\n';
    std::cout << " - maxTextureDimension3D: " << SupportedLimits.maxTextureDimension3D << '\n';
    std::cout << " - maxTextureArrayLayers: " << SupportedLimits.maxTextureArrayLayers << '\n';
  }
#endif
}

}// namespace

int main()
{
  // We create a descriptor
  constexpr int KTimedWaitAnyMaxCount = 20;
  WGPUInstanceDescriptor Desc = { .nextInChain = nullptr,
    .capabilities = {
      .nextInChain = nullptr, .timedWaitAnyEnable = 1U, .timedWaitAnyMaxCount = KTimedWaitAnyMaxCount } };
  Desc.nextInChain = nullptr;

  // We create the instance using this descriptor
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
  WGPUInstance instance = wgpuCreateInstance(nullptr);
#else//  WEBGPU_BACKEND_EMSCRIPTEN
  WGPUInstance Instance = wgpuCreateInstance(&Desc);
#endif//  WEBGPU_BACKEND_EMSCRIPTEN
  if (Instance == nullptr) {
    std::puts("Could not initialize WebGPU!\n");
    return 1;
  }
  std::cout << "WGPU instance: " << Instance << '\n';
  std::cout << "Requesting adapter..." << '\n';

  WGPURequestAdapterOptions AdapterOpts = {};
  AdapterOpts.nextInChain = nullptr;
  WGPUAdapter Adapter = requestAdapterSync(Instance, &AdapterOpts);

  std::cout << "Got adapter: " << Adapter << '\n';
  wgpuInstanceRelease(Instance);
  inspectAdapter(Adapter);
  wgpuAdapterRelease(Adapter);
}
