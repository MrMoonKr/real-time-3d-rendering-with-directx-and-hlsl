#pragma once

#include "Camera.h"

namespace Library
{
    class GameTime;

    class PerspectiveCamera : public Camera
    {
		RTTI_DECLARATIONS(PerspectiveCamera, Camera)

    public:
		explicit PerspectiveCamera(Game& game, float fieldOfView = DefaultFieldOfView, float aspectRatio = DefaultAspectRatio, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		PerspectiveCamera(const PerspectiveCamera&) = default;
		PerspectiveCamera& operator=(const PerspectiveCamera&) = default;
		PerspectiveCamera(PerspectiveCamera&&) = default;
		PerspectiveCamera& operator=(PerspectiveCamera&&) = default;
		virtual ~PerspectiveCamera() = default;
       
        float AspectRatio() const;
        float FieldOfView() const;

		virtual void UpdateProjectionMatrix() override;
        
		inline static const float DefaultFieldOfView{ DirectX::XM_PIDIV4 };
		inline static const float DefaultAspectRatio{ 4.0f / 3.0f };

    protected:
        float mFieldOfView;
        float mAspectRatio;
    };
}