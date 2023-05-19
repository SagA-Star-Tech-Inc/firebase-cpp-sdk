// Copyright 2019 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <inttypes.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "app_framework.h"            // NOLINT
#include "firebase_test_framework.h"  // NOLINT

// The TO_STRING macro is useful for command line defined strings as the quotes
// get stripped.
#define TO_STRING_EXPAND(X) #X
#define TO_STRING(X) TO_STRING_EXPAND(X)

// Path to the Firebase config file to load.
#ifdef FIREBASE_CONFIG
#define FIREBASE_CONFIG_STRING TO_STRING(FIREBASE_CONFIG)
#else
#define FIREBASE_CONFIG_STRING ""
#endif  // FIREBASE_CONFIG

namespace firebase_testapp_automated {

using firebase_test_framework::FirebaseTest;

class FirebaseAppTest : public FirebaseTest {
 public:
  FirebaseAppTest();
};

FirebaseAppTest::FirebaseAppTest() {
  FindFirebaseConfig(FIREBASE_CONFIG_STRING);
}

// For simplicity of test code, handle the Android-specific arguments here.
#if defined(__ANDROID__)
#define APP_CREATE_PARAMS \
  app_framework::GetJniEnv(), app_framework::GetActivity()
#else
#define APP_CREATE_PARAMS
#endif  // defined(__ANDROID__)

TEST_F(FirebaseAppTest, TestDefaultAppWithDefaultOptions) {
  firebase::App* default_app;
  default_app = firebase::App::Create(APP_CREATE_PARAMS);
  EXPECT_NE(default_app, nullptr);

  delete default_app;
  default_app = nullptr;
}

TEST_F(FirebaseAppTest, TestGetAll) {
  std::vector<firebase::App*> all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 0);

  firebase::App* default_app;
  default_app = firebase::App::Create(APP_CREATE_PARAMS);

  all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 1);

  firebase::App* second_app;
  second_app = firebase::App::Create(firebase::AppOptions(), "2ndApp");

  all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 2);

  firebase::App* third_app;
  third_app = firebase::App::Create(firebase::AppOptions(), "3rdApp");

  all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 3);

  delete third_app;
  third_app = nullptr;
  all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 2);

  delete default_app;
  default_app = nullptr;
  all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 1);

  delete second_app;
  second_app = nullptr;

  all_apps = firebase::App::GetApps();
  EXPECT_EQ(all_apps.size(), 0);
}

}  // namespace firebase_testapp_automated
