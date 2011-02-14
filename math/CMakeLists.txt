#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta math library.
#
#

set( sources
     ConvergenceException.cpp
     NullVectorException.cpp
     SingularMatrixException.cpp
     DivMod.cpp
     Shift.cpp
     FixedPoint.cpp
     Polynomial.cpp
     RootFinder.cpp
     Angle.cpp
     AngleDMS.cpp
     AngleHMS.cpp
     Point2.cpp
     Vector.cpp
     Vector2.cpp
     Matrix2.cpp
     RotationMatrix2.cpp
     Polar.cpp
     Point3.cpp
     Vector3.cpp
     Matrix3.cpp
     RotationMatrix3.cpp
     Quaternion.cpp
     EulerAngles.cpp
     Spherical.cpp
     Vector4.cpp
     Matrix4.cpp
     Random.cpp
     Factorial.cpp
     Gamma.cpp
     ProbabilityDistributions.cpp
     StatisticalTests.cpp
     Permutation.cpp
     ODE.cpp
   )

set( EpsilonDeltaLibs
     util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_math  STATIC  ${sources} )
