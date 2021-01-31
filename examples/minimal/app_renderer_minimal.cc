// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "examples/shared/app_factory.h"

namespace shared {

// Implementation of CefApp for the renderer process.
class RendererApp : public CefApp, public CefRenderProcessHandler {
 public:
  RendererApp() {}

  // CefApp methods:
  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
    return this;
  }

  // CefRenderProcessHandler methods:
  void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
    std::string extensionCode =
        "function __init() {\n"
        "var handler3 = {\n"
        "  get: function (target, prop, receiver) {\n"
        "    console.log('Getter', prop);\n"
        "    return Reflect.get(...arguments);\n"
        "  },\n"
        "};\n"
        "\n"
        "window.chrome = new Proxy(window.chrome, handler3);\n"
        "}\n"
        "\n"
        "document.addEventListener('DOMContentLoaded', __init);\n";
    if (frame->IsMain()) {
      frame->ExecuteJavaScript(extensionCode, CefString(), 0);
    }
  }

 private:

  IMPLEMENT_REFCOUNTING(RendererApp);
  DISALLOW_COPY_AND_ASSIGN(RendererApp);
};

// No CefApp for the renderer subprocess.
CefRefPtr<CefApp> CreateRendererProcessApp() {
  return new RendererApp();
}

}  // namespace shared
