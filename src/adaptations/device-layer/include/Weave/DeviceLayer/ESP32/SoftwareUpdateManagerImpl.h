/*
 *
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef SOFTWARE_UPDATE_MANAGER_IMPL_H
#define SOFTWARE_UPDATE_MANAGER_IMPL_H

#if WEAVE_DEVICE_CONFIG_ENABLE_SOFTWARE_UPDATE_MANAGER

#include <Weave/DeviceLayer/internal/GenericSoftwareUpdateManagerImpl.h>
#include <Weave/DeviceLayer/internal/GenericSoftwareUpdateManagerImpl_BDX.h>

namespace nl {
namespace Weave {
namespace DeviceLayer {

/**
 * Concrete implementation of the SoftwareUpdateManager singleton object for the
 * ESP32 platforms.
 */
class SoftwareUpdateManagerImpl final
    : public SoftwareUpdateManager,
      public Internal::GenericSoftwareUpdateManagerImpl<SoftwareUpdateManagerImpl>,
      public Internal::GenericSoftwareUpdateManagerImpl_BDX<SoftwareUpdateManagerImpl>
{
    // Allow the SoftwareUpdateManager interface class to delegate method calls to
    // the implementation methods provided by this class.
    friend class SoftwareUpdateManager;

    // Allow the GenericSoftwareUpdateManagerImpl base class to access helper methods
    // and types defined on this class.
    friend class Internal::GenericSoftwareUpdateManagerImpl<SoftwareUpdateManagerImpl>;

    // Allow the GenericSoftwareUpdateManagerImpl_BDX base class to access helper methods
    // and types defined on this class.
    friend class Internal::GenericSoftwareUpdateManagerImpl_BDX<SoftwareUpdateManagerImpl>;

public:

    // ===== Members for internal use by the following friends.

    friend ::nl::Weave::DeviceLayer::SoftwareUpdateManager & SoftwareUpdateMgr(void);
    friend SoftwareUpdateManagerImpl & SoftwareUpdateMgrImpl(void);

    static SoftwareUpdateManagerImpl sInstance;

private:
    // ===== Members that implement the SoftwareUpdateManager abstract interface.

    WEAVE_ERROR _Init(void);
    WEAVE_ERROR StartImageDownload(char *aURI, uint64_t aStartOffset);
    WEAVE_ERROR GetUpdateSchemeList(::nl::Weave::Profiles::SoftwareUpdate::UpdateSchemeList * aUpdateSchemeList);
    WEAVE_ERROR AbortDownload(void);

    esp_err_t _http_event_handle(esp_http_client_event_t *evt)

};

/**
 * Returns a reference to the public interface of the SoftwareUpdateManager singleton object.
 *
 * Internal components should use this to access features of the SoftwareUpdateManager object
 * that are common to all platforms.
 */
inline SoftwareUpdateManager & SoftwareUpdateMgr(void)
{
    return SoftwareUpdateManagerImpl::sInstance;
}

/**
 * Returns the platform-specific implementation of the SoftwareUpdateManager singleton object.
 *
 * Internal components can use this to gain access to features of the SoftwareUpdateManager
 * that are specific to the ESP32 platform.
 */
inline SoftwareUpdateManagerImpl & SoftwareUpdateMgrImpl(void)
{
    return SoftwareUpdateManagerImpl::sInstance;
}

} // namespace DeviceLayer
} // namespace Weave
} // namespace nl

#endif // WEAVE_DEVICE_CONFIG_ENABLE_SOFTWARE_UPDATE_MANAGER
#endif // SOFTWARE_UPDATE_MANAGER_IMPL_H
