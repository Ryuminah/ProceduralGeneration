
#include "FBXLoader.h"

CFBXLoader::CFBXLoader()	:
	m_Manager(nullptr),
	m_Scene(nullptr),
	m_Mixamo(false)
{
}

CFBXLoader::~CFBXLoader()
{
	size_t	Size = m_vecMaterials.size();

	for (size_t i = 0; i < Size; ++i)
	{
		size_t	Size1 = m_vecMaterials[i].size();

		for (size_t j = 0; j < Size1; ++j)
		{
			SAFE_DELETE(m_vecMaterials[i][j]);
		}
	}

	m_vecMaterials.clear();

	Size = m_vecMeshContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();

	for (int i = 0; i < m_NameArr.Size(); ++i)
	{
		m_NameArr[i]->Clear();
		SAFE_DELETE(m_NameArr[i]);
	}

	m_NameArr.Clear();

	Size = m_vecBones.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecBones[i]);
	}

	m_vecBones.clear();

	Size = m_vecClip.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecClip[i]);
	}

	m_vecClip.clear();

	m_Scene->Clear();
	m_Scene->Destroy();
	m_Manager->Destroy();
}

bool CFBXLoader::LoadFBX(const char* FullPath, bool Static)
{
	// FbxManager ��ü�� �����Ѵ�.
	m_Manager = FbxManager::Create();

	// IOSetting�� �����Ѵ�.
	FbxIOSettings* Ios = FbxIOSettings::Create(m_Manager, IOSROOT);

	// FbxManager�� �����Ѵ�.
	m_Manager->SetIOSettings(Ios);

	// FbxScene�� �����Ѵ�.
	m_Scene = FbxScene::Create(m_Manager, "");

	// FbxImporter �� �̿��ؼ� fbx ���Ϸκ��� �޽� ������ �о�´�.
	FbxImporter* Importer = FbxImporter::Create(m_Manager, "");

	// �ش� ��ο� �ִ� fbx ������ �о���� ������ ������.
	Importer->Initialize(FullPath, -1, m_Manager->GetIOSettings());

	// ������ ���� ������ FbxScene�� ��带 �����Ѵ�.
	Importer->Import(m_Scene);

	if (m_Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	if (!Static)
	{
		m_Scene->FillAnimStackNameArray(m_NameArr);

		if (m_NameArr.GetCount() > 0)
		{
			LoadAnimationClip();

			// �� ������ �о�´�.
			LoadBone(m_Scene->GetRootNode());

			// Ŭ���� ������ �ִ� �������� �� ������ŭ resize ���ش�.
			// ���� Animation Clip���� �ϴ����̴�.
			for (size_t i = 0; i < m_vecClip.size(); ++i)
			{
				m_vecClip[i]->vecBoneKeyFrame.resize(m_vecBones.size());
			}
		}
	}

	Triangulate(m_Scene->GetRootNode());

	LoadMesh(m_Scene->GetRootNode(), Static);

	Importer->Destroy();

	return true;
}

void CFBXLoader::Triangulate(FbxNode* pNode)
{
	// FbxNodeAttribute : FbxNode �Ӽ��� ��Ÿ���� �������̽��̴�.
	// ���κ��� ���� �� �ִ�.
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter	converter(m_Manager);

		converter.Triangulate(pAttr, true);
	}

	// ���������� �д´�.
	int	iMtrlCount = pNode->GetMaterialCount();

	if (iMtrlCount > 0)
	{
		std::vector<PFBXMATERIAL>	vecMtrl;

		m_vecMaterials.push_back(vecMtrl);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			// FbxSurfaceMaterial : Fbx �� ������ �ִ� ���� ������
			// ����ִ�.
			FbxSurfaceMaterial* pMtrl = pNode->GetMaterial(i);

			if (!pMtrl)
				continue;

			LoadMaterial(pMtrl);
		}
	}

	// ���� ����� �ڽĳ�� ���� ���´�.
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(pNode->GetChild(i));
	}
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* pMtrl)
{
	// ���� ������ ������ ����ü�� �����Ѵ�.
	PFBXMATERIAL	pMtrlInfo = new FBXMATERIAL;

	// ���� ������ �����̳ʿ� ���� ������ �߰��Ѵ�.
	m_vecMaterials[m_vecMaterials.size() - 1].push_back(pMtrlInfo);

	// Diffuse ������ �о�´�.
	pMtrlInfo->Dif = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sDiffuse,
		FbxSurfaceMaterial::sDiffuseFactor);

	pMtrlInfo->Amb = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sAmbient,
		FbxSurfaceMaterial::sAmbientFactor);

	pMtrlInfo->Spc = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sSpecular,
		FbxSurfaceMaterial::sSpecularFactor);

	pMtrlInfo->Emv = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sEmissive,
		FbxSurfaceMaterial::sEmissiveFactor);

	pMtrlInfo->SpecularPower = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Shininess = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sShininess);
	pMtrlInfo->TransparencyFactor = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sTransparencyFactor);

	pMtrlInfo->DifTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sDiffuse);
	pMtrlInfo->BumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sNormalMap);

	if (pMtrlInfo->BumpTex.empty())
		pMtrlInfo->BumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sBump);

	pMtrlInfo->SpcTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 CFBXLoader::GetMaterialColor(FbxSurfaceMaterial* pMtrl, const char* pPropertyName, 
	const char* pPropertyFactorName)
{
	FbxDouble3	vResult(0, 0, 0);
	double		dFactor = 0;

	// ���ڷ� ���� �������� �ش� �̸��� ���� ������Ƽ�� �ִ����� �Ǵ��Ѵ���
	// ���´�. ���������� �ش� �̸��� ������Ƽ ���Ͱ� �ִ����� �Ǵ��Ѵ���
	// ���´�.
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	FbxProperty	tPropertyFactor = pMtrl->FindProperty(pPropertyFactorName);

	// ��ȿ���� üũ�Ѵ�.
	if (tProperty.IsValid() && tPropertyFactor.IsValid())
	{
		vResult = tProperty.Get<FbxDouble3>();
		dFactor = tPropertyFactor.Get<FbxDouble>();

		// Factor�� 1�� �ƴ� ��� ������ ���������� ����Ͽ�
		// ���� vResult �� ��� �����ش�.
		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return Vector4((float)vResult[0], (float)vResult[1], (float)vResult[2], (float)dFactor);
}

double CFBXLoader::GetMaterialFactor(FbxSurfaceMaterial* pMtrl, const char* pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	double	dFactor = 0.0;

	if (tProperty.IsValid())
		dFactor = tProperty.Get<FbxDouble>();

	return dFactor;
}

std::string CFBXLoader::GetMaterialTexture(FbxSurfaceMaterial* pMtrl, const char* pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	std::string	str = "";

	if (tProperty.IsValid())
	{
		// FbxFileTexture �� Ÿ���� ��� �ִ����� ������ �Լ��̴�.
		int	iTexCount = tProperty.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture* pFileTex = tProperty.GetSrcObject<FbxFileTexture>(0);

			if (pFileTex)
				str = pFileTex->GetFileName();
		}
	}

	return str;
}

void CFBXLoader::LoadMesh(FbxNode* pNode, bool bStatic)
{
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = pNode->GetMesh();

		if (pMesh)
			LoadMesh(pMesh, bStatic);
	}

	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadMesh(pNode->GetChild(i), bStatic);
	}
}

void CFBXLoader::LoadMesh(FbxMesh* pMesh, bool bStatic)
{
	// MeshContainer�� ����� �����̳ʺ� ������ �����̳�&����� ��
	// �ε��� ������ ���ͼ� �޽��� �����ؾ� �Ѵ�.
	PFBXMESHCONTAINER	pContainer = new FBXMESHCONTAINER;

	pContainer->Bump = false;

	m_vecMeshContainer.push_back(pContainer);

	// ControlPoint �� ��ġ������ ��� �ִ� �迭�̴�.
	// �� �迭�� ������ �� ������ ������ �ȴ�.
	int	iVtxCount = pMesh->GetControlPointsCount();

	// ���������� FbxVector4Ÿ���� �迭�� �����ϰ� �ֱ� ������ �迭�� 
	// �����ּҸ� ���´�.
	FbxVector4* pVtxPos = pMesh->GetControlPoints();

	// �����̳ʰ� ������ �ִ� ���� �������� ��������ŭ resize ���ش�.
	pContainer->vecPos.resize(iVtxCount);
	pContainer->vecNormal.resize(iVtxCount);
	pContainer->vecUV.resize(iVtxCount);
	pContainer->vecTangent.resize(iVtxCount);
	pContainer->vecBinormal.resize(iVtxCount);

	for (int i = 0; i < iVtxCount; ++i)
	{
		// y�� z���� �ٲ�� �ֱ� ������ �������ش�.
		pContainer->vecPos[i].x = (float)pVtxPos[i].mData[0];
		pContainer->vecPos[i].y = (float)pVtxPos[i].mData[2];
		pContainer->vecPos[i].z = (float)pVtxPos[i].mData[1];
	}

	// ������ ���� ���´�.
	int	iPolygonCount = pMesh->GetPolygonCount();

	UINT	iVtxID = 0;

	// ���� ���� ���´�.(���� �� == ����� ��)
	int	iMtrlCount = pMesh->GetNode()->GetMaterialCount();

	iMtrlCount = iMtrlCount == 0 ? 1 : iMtrlCount;

	// ���� ���� �� ����� ���̱� ������ ���� ����ŭ resize �Ѵ�.
	pContainer->vecIndices.resize(iMtrlCount);

	// ���� ������ ���´�.
	FbxGeometryElementMaterial* pMaterial = pMesh->GetElementMaterial();
	int iCount = pMesh->GetElementMaterialCount();
	// �ﰢ�� ����ŭ �ݺ��Ѵ�.
	for (int i = 0; i < iPolygonCount; ++i)
	{
		// �� �������� �����ϴ� ������ ���� ���´�.
		// �ﰢ���� ��� 3���� ���´�.
		int	iPolygonSize = pMesh->GetPolygonSize(i);

		int	iIdx[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// ���� �ﰢ���� �����ϰ� �ִ� ���ؽ����� �������� �ε�����
			// ���Ѵ�.
			int	iControlIndex = pMesh->GetPolygonVertex(i, j);

			iIdx[j] = iControlIndex;

			LoadNormal(pMesh, pContainer, iVtxID, iControlIndex);

			LoadUV(pMesh, pContainer, pMesh->GetTextureUVIndex(i, j),
				iControlIndex);

			LoadTangent(pMesh, pContainer, iVtxID, iControlIndex);

			LoadBinormal(pMesh, pContainer, iVtxID, iControlIndex);

			++iVtxID;
		}

		// �ﰢ���� �Ҽӵ� ����� �ε����� ���´�.
		int	iMtrlID = 0;

		if (pMaterial)
			iMtrlID = pMaterial->GetIndexArray().GetAt(i);

		pContainer->vecIndices[iMtrlID].push_back(iIdx[0]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[2]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[1]);
	}

	if (!bStatic)
		LoadAnimation(pMesh, pContainer);
}

void CFBXLoader::LoadNormal(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	// �޽��κ��� ElementNormal ������ ���´�.
	FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal();

	int	iNormalIndex = iVtxID;

	// MappingMode�� ReferenceMode�� ���� �ε����� ����� ������ �޶�����.
	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iNormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);

	pContainer->vecNormal[iControlIndex].x = (float)vNormal.mData[0];
	pContainer->vecNormal[iControlIndex].y = (float)vNormal.mData[2];
	pContainer->vecNormal[iControlIndex].z = (float)vNormal.mData[1];
}

void CFBXLoader::LoadUV(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer, int iUVID, int iControlIndex)
{
	int iCount = pMesh->GetElementUVCount();
	FbxGeometryElementUV* pUV = pMesh->GetElementUV(0);

	if (!pUV)
		return;

	int	iUVIndex = iUVID;

	if (pUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iUVIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iUVIndex = pUV->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	else if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			//iUVIndex = pUV->GetIndexArray().GetAt(iUVIndex);
		}
		break;
		default:
			break; // other reference modes not shown here!
		}
	}

	FbxVector2	vUV = pUV->GetDirectArray().GetAt(iUVIndex);

	pContainer->vecUV[iControlIndex].x = (float)vUV.mData[0] - (int)vUV.mData[0];
	pContainer->vecUV[iControlIndex].y = 1.f - (float)(vUV.mData[1] - (int)vUV.mData[1]);
}

void CFBXLoader::LoadTangent(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementTangent* pTangent = pMesh->GetElementTangent();

	if (!pTangent)
		return;

	pContainer->Bump = true;

	int	iTangentIndex = iVtxID;

	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iTangentIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);

	pContainer->vecTangent[iControlIndex].x = (float)vTangent.mData[0];
	pContainer->vecTangent[iControlIndex].y = (float)vTangent.mData[2];
	pContainer->vecTangent[iControlIndex].z = (float)vTangent.mData[1];
}

void CFBXLoader::LoadBinormal(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementBinormal* pBinormal = pMesh->GetElementBinormal();

	if (!pBinormal)
		return;

	pContainer->Bump = true;

	int	iBinormalIndex = iVtxID;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iBinormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);

	pContainer->vecBinormal[iControlIndex].x = (float)vBinormal.mData[0];
	pContainer->vecBinormal[iControlIndex].y = (float)vBinormal.mData[2];
	pContainer->vecBinormal[iControlIndex].z = (float)vBinormal.mData[1];
}

void CFBXLoader::LoadAnimationClip()
{
	int	iCount = m_NameArr.GetCount();

	FbxTime::EMode	eTimeMode = m_Scene->GetGlobalSettings().GetTimeMode();

	for (int i = 0; i < iCount; ++i)
	{
		// m_NameArr�� ����� �̸����� Scene���κ��� FbxAnimStack ��ü�� ���´�.
		FbxAnimStack* pAnimStack = m_Scene->FindMember<FbxAnimStack>(m_NameArr[i]->Buffer());

		if (!pAnimStack)
			continue;

		PFBXANIMATIONCLIP	pClip = new FBXANIMATIONCLIP;

		pClip->strName = pAnimStack->GetName();

		if (pClip->strName == "mixamo.com")
			m_Mixamo = true;

		FbxTakeInfo* pTake = m_Scene->GetTakeInfo(pClip->strName.c_str());

		pClip->tStart = pTake->mLocalTimeSpan.GetStart();
		pClip->tEnd = pTake->mLocalTimeSpan.GetStop();
		// GetFrameCount �Լ��� ȣ���ϰ�  time��带 �־��ָ� �ð��� ����������
		// ��ȯ���ش�. �������� ¥�� �ִϸ��̼� ������ �����ش�.
		pClip->lTimeLength = pClip->tEnd.GetFrameCount(eTimeMode) -
			pClip->tStart.GetFrameCount(eTimeMode);
		pClip->eTimeMode = eTimeMode;

		m_vecClip.push_back(pClip);
	}
}

void CFBXLoader::LoadBone(FbxNode* pNode)
{
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadBoneRecursive(pNode->GetChild(i), 0, 0, -1);
	}
}

void CFBXLoader::LoadBoneRecursive(FbxNode* pNode, int iDepth, int iIndex, int iParent)
{
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() ==
		FbxNodeAttribute::eSkeleton)
	{
		PFBXBONE	pBone = new FBXBONE;

		pBone->strName = pNode->GetName();
		if (m_Mixamo)
			pBone->strName.erase(0, 10);
		pBone->iDepth = iDepth;
		pBone->iParentIndex = iParent;

		m_vecBones.push_back(pBone);
	}

	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		LoadBoneRecursive(pNode->GetChild(i), iDepth + 1,
			m_vecBones.size(), iIndex);
	}
}

void CFBXLoader::LoadAnimation(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer)
{
	int	iSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (iSkinCount <= 0)
		return;

	// �޽��� ���� ���� ���´�.
	int	iCPCount = pMesh->GetControlPointsCount();

	// ������ ����ġ ������ ���ε��� ������ ���� ����ŭ
	// ��������� �Ѵ�.
	pContainer->vecBlendWeight.resize(iCPCount);
	pContainer->vecBlendIndex.resize(iCPCount);

	pContainer->Animation = true;

	FbxAMatrix	matTransform = GetTransform(pMesh->GetNode());

	for (int i = 0; i < iSkinCount; ++i)
	{
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (!pSkin)
			continue;

		FbxSkin::EType	eSkinningType = pSkin->GetSkinningType();

		if (eSkinningType == FbxSkin::eRigid ||
			eSkinningType == FbxSkin::eLinear ||
			eSkinningType == FbxSkin::eBlend)
		{
			// Cluster : ������ �ǹ��Ѵ�.
			int	iClusterCount = pSkin->GetClusterCount();

			for (int j = 0; j < iClusterCount; ++j)
			{
				FbxCluster* pCluster = pSkin->GetCluster(j);

				if (!pCluster->GetLink())
					continue;

				std::string	strBoneName = pCluster->GetLink()->GetName();

				if (m_Mixamo)
					strBoneName.erase(0, 10);

				int	iBoneIndex = FindBoneFromName(strBoneName);

				LoadWeightAndIndex(pCluster, iBoneIndex, pContainer);

				LoadOffsetMatrix(pCluster, matTransform, iBoneIndex, pContainer);

				m_vecBones[iBoneIndex]->matBone = matTransform;

				LoadTimeTransform(pMesh->GetNode(), pCluster,
					matTransform, iBoneIndex);
			}
		}
	}

	ChangeWeightAndIndices(pContainer);
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* pNode)
{
	const FbxVector4	vT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4	vR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4	vS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

int CFBXLoader::FindBoneFromName(const std::string& strName)
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strName)
			return i;
	}

	return -1;
}

void CFBXLoader::LoadWeightAndIndex(FbxCluster* pCluster, int iBoneIndex, PFBXMESHCONTAINER pContainer)
{
	int	iControlIndicesCount = pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iControlIndicesCount; ++i)
	{
		FBXWEIGHT	tWeight = {};

		tWeight.iIndex = iBoneIndex;
		tWeight.dWeight = pCluster->GetControlPointWeights()[i];

		int	iClusterIndex = pCluster->GetControlPointIndices()[i];

		// map�� Ư¡ : Ű�� �̿��� �ε���ó�� ������ ��� �ش� Ű�� ���ٸ�
		// ������ش�.
		pContainer->mapWeights[iClusterIndex].push_back(tWeight);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster* pCluster,
	const FbxAMatrix& matTransform, int iBoneIndex,
	PFBXMESHCONTAINER pContainer)
{
	FbxAMatrix	matCluster;
	FbxAMatrix	matClusterLink;

	pCluster->GetTransformMatrix(matCluster);
	pCluster->GetTransformLinkMatrix(matClusterLink);

	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	/*
	1 0 0 0   1 2 3 4   1 0 0 0
	0 0 1 0   5 6 7 8   0 0 1 0
	0 1 0 0 * 9 0 1 2 * 0 1 0 0
	0 0 0 1   3 4 5 6   0 0 0 1

	1 2 3 4   1 0 0 0
	9 0 1 2   0 0 1 0
	5 6 7 8 * 0 1 0 0
	3 4 5 6   0 0 0 1

	1 3 2 4
	9 1 0 2
	5 7 6 8
	3 5 4 6
	*/

	FbxAMatrix	matOffset;
	matOffset = matClusterLink.Inverse() * matCluster * matTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBones[iBoneIndex]->matOffset = matOffset;
}

void CFBXLoader::LoadTimeTransform(FbxNode* pNode,
	FbxCluster* pCluster, const FbxAMatrix& matTransform,
	int iBoneIndex)
{
	FbxVector4	v1 = { 1.0, 0.0, 0.0, 0.0 };
	FbxVector4	v2 = { 0.0, 0.0, 1.0, 0.0 };
	FbxVector4	v3 = { 0.0, 1.0, 0.0, 0.0 };
	FbxVector4	v4 = { 0.0, 0.0, 0.0, 1.0 };

	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	if (m_Mixamo)
	{
		std::vector<PFBXANIMATIONCLIP>::iterator	iter;
		std::vector<PFBXANIMATIONCLIP>::iterator	iterEnd = m_vecClip.end();

		for (iter = m_vecClip.begin(); iter != iterEnd;)
		{
			if ((*iter)->strName != "mixamo.com")
			{
				SAFE_DELETE((*iter));
				iter = m_vecClip.erase(iter);
				iterEnd = m_vecClip.end();
			}

			else
				++iter;
		}
	}

	for (size_t i = 0; i < m_vecClip.size(); ++i)
	{
		FbxLongLong	Start = m_vecClip[i]->tStart.GetFrameCount(m_vecClip[i]->eTimeMode);
		FbxLongLong	End = m_vecClip[i]->tEnd.GetFrameCount(m_vecClip[i]->eTimeMode);

		m_vecClip[i]->vecBoneKeyFrame[iBoneIndex].iBoneIndex = iBoneIndex;

		// ��ü ������ ����ŭ �ݺ��Ѵ�.
		for (FbxLongLong j = Start; j <= End; ++j)
		{
			FbxTime	tTime = {};

			// ���� �����ӿ� �ش��ϴ� FbxTime�� ������.
			tTime.SetFrame(j, m_vecClip[i]->eTimeMode);

			// EvaluateGlobalTransform
			FbxAMatrix	matOffset = pNode->EvaluateGlobalTransform(tTime) * matTransform;
			FbxAMatrix	matCur = matOffset.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(tTime);

			matCur = matReflect * matCur * matReflect;

			FBXKEYFRAME	tKeyFrame = {};

			tKeyFrame.dTime = tTime.GetSecondDouble();
			tKeyFrame.matTransform = matCur;

			m_vecClip[i]->vecBoneKeyFrame[iBoneIndex].vecKeyFrame.push_back(tKeyFrame);
		}
	}
}

void CFBXLoader::ChangeWeightAndIndices(PFBXMESHCONTAINER pContainer)
{
	std::unordered_map<int, std::vector<FBXWEIGHT>>::iterator	iter;
	std::unordered_map<int, std::vector<FBXWEIGHT>>::iterator	iterEnd = pContainer->mapWeights.end();

	for (iter = pContainer->mapWeights.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second.size() > 4)
		{
			// ����ġ ���� ���� �������� �����Ѵ�.
			sort(iter->second.begin(), iter->second.end(), [](const FBXWEIGHT& lhs, const FBXWEIGHT& rhs)
				{
					return lhs.dWeight > rhs.dWeight;
				});

			double	dSum = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				dSum += iter->second[i].dWeight;
			}

			double	dInterpolate = 1.f - dSum;

			std::vector<FBXWEIGHT>::iterator	iterErase = iter->second.begin() + 4;

			iter->second.erase(iterErase, iter->second.end());
			iter->second[0].dWeight += dInterpolate;
		}

		Vector4	vWeight;
		Vector4	vIndex;

		for (int i = 0; i < iter->second.size(); ++i)
		{
			vWeight[i] = iter->second[i].dWeight;
			vIndex[i] = iter->second[i].iIndex;
		}

		pContainer->vecBlendWeight[iter->first] = vWeight;
		pContainer->vecBlendIndex[iter->first] = vIndex;
	}
}
