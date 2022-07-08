/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making libpag available.
//
//  Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  unless required by applicable law or agreed to in writing, software distributed under the
//  license is distributed on an "as is" basis, without warranties or conditions of any kind,
//  either express or implied. see the license for the specific language governing permissions
//  and limitations under the license.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "tgfx/core/Mesh.h"
#include "PathRef.h"
#include "SimplePathMesh.h"
#include "TriangularPathMesh.h"

namespace tgfx {
std::unique_ptr<Mesh> Mesh::MakeFrom(const Path& path, const Rect* clipBounds) {
  Rect rect;
  if (path.asRect(&rect)) {
    return std::make_unique<SimplePathMesh>(rect);
  }
  RRect rRect;
  if (path.asRRect(&rRect)) {
    return std::make_unique<SimplePathMesh>(rRect);
  }
  const auto& skPath = PathRef::ReadAccess(path);
  std::vector<float> vertices;
  auto skRect = clipBounds ? pk::SkRect::MakeLTRB(clipBounds->left, clipBounds->top,
                                                  clipBounds->right, clipBounds->bottom)
                           : skPath.getBounds();
  int count = skPath.toAATriangles(DefaultTolerance, skRect, &vertices);
  if (count == 0) {
    return nullptr;
  }
  return std::make_unique<TriangularPathMesh>(std::move(vertices), count, path.getBounds());
}
}  // namespace tgfx