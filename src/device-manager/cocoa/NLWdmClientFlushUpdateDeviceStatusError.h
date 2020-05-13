/*
 *
 *    Copyright (c) 2020 Google, LLC.
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

/**
 *    @file
 *      Objective-C representation of a device status for WdmClient flush update operation
 *
 */

#import <Foundation/Foundation.h>

#import "NLProfileStatusError.h"
#import "NLGenericTraitUpdatableDataSink.h"
#import "NLGenericTraitUpdatableDataSink.h"

@interface NLWdmClientFlushUpdateDeviceStatusError : NLProfileStatusError

@property (nonatomic, readonly) NSString * path;
@property (nonatomic, readonly) NLGenericTraitUpdatableDataSink * dataSink;

- (id)initWithProfileId:(uint32_t)profileId
             statusCode:(uint16_t)statusCode
              errorCode:(uint32_t)errorCode
           statusReport:(NSString *)statusReport
                   path:(NSString *)path
               dataSink:(NLGenericTraitUpdatableDataSink *)dataSink;

@end
