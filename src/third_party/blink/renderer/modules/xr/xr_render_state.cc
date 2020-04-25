// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/modules/xr/xr_render_state.h"

#include <algorithm>

#include "third_party/blink/renderer/modules/xr/xr_layer.h"
#include "third_party/blink/renderer/modules/xr/xr_presentation_context.h"
#include "third_party/blink/renderer/modules/xr/xr_render_state_init.h"

namespace blink {

namespace {
// The WebXR spec specifies that the min and max are up the UA, but have to be
// within 0 and Pi.  Using those exact numbers can lead to floating point math
// errors, so set them slightly inside those numbers.
constexpr double kMinFieldOfView = 0.01;
constexpr double kMaxFieldOfView = 3.13;
constexpr double kDefaultFieldOfView = M_PI * 0.5;
}  // anonymous namespace

XRRenderState::XRRenderState(bool immersive) : immersive_(immersive) {
  if (!immersive_)
    inline_vertical_fov_ = kDefaultFieldOfView;
}

void XRRenderState::Update(const XRRenderStateInit* init) {
  if (init->hasDepthNear()) {
    depth_near_ = init->depthNear();
  }
  if (init->hasDepthFar()) {
    depth_far_ = init->depthFar();
  }
  if (init->hasBaseLayer()) {
    base_layer_ = init->baseLayer();
  }
  if (init->hasOutputContext()) {
    output_context_ = init->outputContext();
  }
  if (init->hasInlineVerticalFieldOfView()) {
    double fov = init->inlineVerticalFieldOfView();

    // Clamp the value between our min and max.
    fov = std::max(kMinFieldOfView, fov);
    fov = std::min(kMaxFieldOfView, fov);
    inline_vertical_fov_ = fov;
  }
}

void XRRenderState::removeOutputContext() {
  output_context_ = nullptr;
}

double XRRenderState::inlineVerticalFieldOfView(bool& is_null) const {
  is_null = immersive_ || !inline_vertical_fov_.has_value();
  return is_null ? 0 : *inline_vertical_fov_;
}

void XRRenderState::Trace(blink::Visitor* visitor) {
  visitor->Trace(base_layer_);
  visitor->Trace(output_context_);
  visitor->Trace(inline_vertical_fov_);
  ScriptWrappable::Trace(visitor);
}

}  // namespace blink