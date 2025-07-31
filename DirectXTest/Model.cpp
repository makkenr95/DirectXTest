#include "pch.h"
#include "Model.h"

CModel& CModel::Init(SModelData aModelData)
{
	myModelData = aModelData;
	return *this;
}

CModel::SModelData CModel::GetModelData() const
{
	return myModelData;
}
