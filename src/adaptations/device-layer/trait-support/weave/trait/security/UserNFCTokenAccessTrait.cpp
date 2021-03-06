
/*
 *    Copyright (c) 2019 Google LLC.
 *    Copyright (c) 2016-2018 Nest Labs, Inc.
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

/*
 *    THIS FILE IS GENERATED. DO NOT MODIFY.
 *
 *    SOURCE TEMPLATE: trait.cpp
 *    SOURCE PROTO: weave/trait/security/user_nfc_token_access_trait.proto
 *
 */

#include <weave/trait/security/UserNFCTokenAccessTrait.h>

namespace Schema {
namespace Weave {
namespace Trait {
namespace Security {
namespace UserNFCTokenAccessTrait {

using namespace ::nl::Weave::Profiles::DataManagement;

//
// Property Table
//

const TraitSchemaEngine::PropertyInfo PropertyMap[] = {
};

//
// Schema
//

const TraitSchemaEngine TraitSchema = {
    {
        kWeaveProfileId,
        PropertyMap,
        sizeof(PropertyMap) / sizeof(PropertyMap[0]),
        1,
#if (TDM_EXTENSION_SUPPORT) || (TDM_VERSIONING_SUPPORT)
        2,
#endif
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#if (TDM_EXTENSION_SUPPORT)
        NULL,
#endif
#if (TDM_VERSIONING_SUPPORT)
        NULL,
#endif
    }
};

    //
    // Events
    //

const nl::FieldDescriptor UserNFCTokenAccessEventFieldDescriptors[] =
{
    {
        NULL, offsetof(UserNFCTokenAccessEvent, result), SET_TYPE_AND_FLAGS(nl::SerializedFieldTypeInt32, 0), 1
    },

    {
        NULL, offsetof(UserNFCTokenAccessEvent, tokenId), SET_TYPE_AND_FLAGS(nl::SerializedFieldTypeUInt64, 0), 2
    },

    {
        NULL, offsetof(UserNFCTokenAccessEvent, userId), SET_TYPE_AND_FLAGS(nl::SerializedFieldTypeByteString, 1), 3
    },

};

const nl::SchemaFieldDescriptor UserNFCTokenAccessEvent::FieldSchema =
{
    .mNumFieldDescriptorElements = sizeof(UserNFCTokenAccessEventFieldDescriptors)/sizeof(UserNFCTokenAccessEventFieldDescriptors[0]),
    .mFields = UserNFCTokenAccessEventFieldDescriptors,
    .mSize = sizeof(UserNFCTokenAccessEvent)
};
const nl::Weave::Profiles::DataManagement::EventSchema UserNFCTokenAccessEvent::Schema =
{
    .mProfileId = kWeaveProfileId,
    .mStructureType = 0x1,
    .mImportance = nl::Weave::Profiles::DataManagement::Production,
    .mDataSchemaVersion = 1,
    .mMinCompatibleDataSchemaVersion = 1,
};

} // namespace UserNFCTokenAccessTrait
} // namespace Security
} // namespace Trait
} // namespace Weave
} // namespace Schema
