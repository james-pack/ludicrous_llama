#pragma once

#include "component/component.h"
#include "component/proto/proto_serialization.h"
#include "position/proto/proto_serialization.h"
#include "render/camera.h"
#include "render/proto/camera.pb.h"
#include "render/proto/scene.pb.h"
#include "render/scene.h"
#include "serialization/serialize.h"

namespace pack {

template <>
inline void to_proto(const render::Camera& camera, render::proto::Camera* proto) {
  proto->set_name(camera.name);
  to_proto(camera.position, proto->mutable_position());
  to_proto(camera.orientation, proto->mutable_orientation());
}

template <>
inline void from_proto(const render::proto::Camera& proto, render::Camera* camera) {
  camera->name = proto.name();
  from_proto(proto.position(), &camera->position);
  from_proto(proto.orientation(), &camera->orientation);
}

template <>
inline void to_proto(const render::Scene& scene, render::proto::Scene* proto) {
  for (const auto& component : scene.root_components) {
    to_proto(component, proto->add_root_components());
  }
  for (const auto& component : scene.components) {
    to_proto(component, proto->add_components());
  }
  for (const auto& camera : scene.cameras) {
    to_proto(camera, proto->add_cameras());
  }
}

template <>
inline void from_proto(const render::proto::Scene& proto, render::Scene* scene) {
  for (const auto& subcomponent : proto.root_components()) {
    scene->root_components.emplace_back(
        from_proto<component::Subcomponent, component::proto::Subcomponent>(subcomponent));
  }
  for (const auto& component : proto.components()) {
    scene->components.insert(from_proto<component::Component, component::proto::Component>(component));
  }
  for (const auto& camera : proto.cameras()) {
    scene->cameras.emplace_back(from_proto<render::Camera, render::proto::Camera>(camera));
  }
}

}  // namespace pack
