#pragma once
#include "InterpolatorFactory.h"
namespace MotionByte
{
	std::shared_ptr<Interpolator> InterpolatorFactory::createStep()
	{
		return std::make_shared<Step>();
	}
	std::shared_ptr<Interpolator> InterpolatorFactory::createLinear(double duration)
	{
		return std::make_shared<Linear>(duration);
	}
	std::shared_ptr<Interpolator> InterpolatorFactory::createEaseInOut(double duration)
	{
		return std::make_shared<EaseInOut>(duration);
	}
	std::shared_ptr<Interpolator> InterpolatorFactory::createSmooth(double acceleration, double expectedVelocity)
	{
		return  std::make_shared<Smooth>(acceleration,expectedVelocity);
	}
}
