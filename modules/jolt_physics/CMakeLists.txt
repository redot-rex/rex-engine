if (NOT DISABLE_PHYSICS_3D AND NOT ARCH STREQUAL "ppc32")
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	set(THIRDPARTY_JOLT_PHYSICS_DIR ${CMAKE_SOURCE_DIR}/thirdparty/jolt_physics/)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/RegisterTypes.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/AABBTree/AABBTreeBuilder.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/Color.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/Factory.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/IssueReporting.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/JobSystemSingleThreaded.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/JobSystemThreadPool.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/JobSystemWithBarrier.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/LinearCurve.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/Memory.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/Profiler.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/RTTI.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/Semaphore.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/StringTools.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Core/TickCounter.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Geometry/ConvexHullBuilder.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Geometry/ConvexHullBuilder2D.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Geometry/Indexify.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Geometry/OrientedBox.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Math/Vec3.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/ObjectStream/SerializableObject.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/DeterminismLog.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/IslandBuilder.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/LargeIslandSplitter.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/PhysicsScene.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/PhysicsSystem.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/PhysicsUpdateContext.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/StateRecorderImpl.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Body/Body.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Body/BodyCreationSettings.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Body/BodyInterface.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Body/BodyManager.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Body/MassProperties.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Body/MotionProperties.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Character/Character.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Character/CharacterBase.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Character/CharacterVirtual.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/CastConvexVsTriangles.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/CastSphereVsTriangles.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/CollideConvexVsTriangles.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/CollideSphereVsTriangles.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/CollisionDispatch.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/CollisionGroup.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/EstimateCollisionResponse.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/GroupFilter.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/GroupFilterTable.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/ManifoldBetweenTwoFaces.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/NarrowPhaseQuery.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/NarrowPhaseStats.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/PhysicsMaterial.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/PhysicsMaterialSimple.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/TransformedShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/BroadPhase/BroadPhase.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/BroadPhase/BroadPhaseBruteForce.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/BroadPhase/BroadPhaseQuadTree.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/BroadPhase/QuadTree.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/BoxShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/CapsuleShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/CompoundShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/ConvexHullShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/ConvexShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/CylinderShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/DecoratedShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/EmptyShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/HeightFieldShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/MeshShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/MutableCompoundShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/PlaneShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/RotatedTranslatedShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/ScaledShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/Shape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/SphereShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/StaticCompoundShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/TaperedCapsuleShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/TaperedCylinderShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Collision/Shape/TriangleShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/ConeConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/Constraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/ConstraintManager.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/ContactConstraintManager.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/DistanceConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/FixedConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/GearConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/HingeConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/MotorSettings.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/PathConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/PathConstraintPath.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/PathConstraintPathHermite.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/PointConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/PulleyConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/RackAndPinionConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/SixDOFConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/SliderConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/SpringSettings.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/SwingTwistConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Constraints/TwoBodyConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Ragdoll/Ragdoll.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/SoftBody/SoftBodyCreationSettings.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/SoftBody/SoftBodyMotionProperties.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/SoftBody/SoftBodyShape.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/SoftBody/SoftBodySharedSettings.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/MotorcycleController.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/TrackedVehicleController.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleAntiRollBar.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleCollisionTester.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleConstraint.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleController.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleDifferential.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleEngine.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleTrack.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/VehicleTransmission.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/Wheel.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Physics/Vehicle/WheeledVehicleController.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Renderer/DebugRenderer.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Renderer/DebugRendererPlayback.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Renderer/DebugRendererRecorder.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Renderer/DebugRendererSimple.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Skeleton/SkeletalAnimation.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Skeleton/Skeleton.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Skeleton/SkeletonMapper.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/Skeleton/SkeletonPose.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/TriangleSplitter/TriangleSplitter.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/TriangleSplitter/TriangleSplitterBinning.cpp
		${THIRDPARTY_JOLT_PHYSICS_DIR}Jolt/TriangleSplitter/TriangleSplitterMean.cpp
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_JOLT_PHYSICS_DIR}
	)
	if (REX_DEV_BUILD)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			JPH_ENABLE_ASSERTS
		)
	endif()
	if (EDITOR_BUILD)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			JPH_DEBUG_RENDERER
		)
	endif()
	if (PRECISION STREQUAL "double")
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			JPH_DOUBLE_PRECISION
		)
	endif()

	file(GLOB_RECURSE JOLT_PHYSICS_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${JOLT_PHYSICS_SOURCES}
	)
endif()

