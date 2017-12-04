#include "ModuleAnimationLoader.h"

#include "Application.h"
#include "ModuleFileSystem.h"

ModuleAnimationLoader::ModuleAnimationLoader(bool start_enabled) : Module(start_enabled)
{
	name = "Animation Loader";
}

ModuleAnimationLoader::~ModuleAnimationLoader()
{
}

bool ModuleAnimationLoader::Init()
{
	return true;
}

bool ModuleAnimationLoader::Start()
{
	return true;
}

update_status ModuleAnimationLoader::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimationLoader::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimationLoader::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleAnimationLoader::CleanUp()
{
	return true;
}

bool ModuleAnimationLoader::Import(const aiAnimation * anim, std::string & output_file)
{
	bool ret = false;
	
	if (anim == nullptr) // Non-Valid Anim
		return false;
	if (anim->mNumChannels < 0)	// No Bones in Anim
		return false;

	Animation new_anim;

	new_anim.name = anim->mName.C_Str();
	new_anim.duration = anim->mDuration;
	new_anim.tickspersec = anim->mTicksPerSecond;
	
	for (uint i = 0; i < anim->mNumChannels; i++) {
		Bone* new_bone = ImportBone(anim->mChannels[i]);
		if(new_bone != nullptr)
			new_anim.Channels.push_back(new_bone);
	}

	return Save(new_anim, output_file);
}

bool ModuleAnimationLoader::Load(const char * file)
{
	bool ret = false;

	return ret;
}

bool ModuleAnimationLoader::Save(const Animation & anim, std::string & output_file)
{
	bool ret = false;

	// Buffer Sizing

	// Buffer Format
	// [Ns N Dur TpS nCh Chnls[]]

	uint buffer_size = 0;
	uint name_size = sizeof(char) * anim.name.length();
	uint dur_size = sizeof(anim.duration);
	uint tps_size = sizeof(anim.tickspersec);
	uint nchnls = anim.NumChannels();
	uint nchnls_size = sizeof(nchnls);

	buffer_size += name_size + dur_size + tps_size + nchnls_size;

	// Bone Buffer Format
	// [Ns N nP Pt Pv nR Rt Rv nS St Sv]
	for (int i = 0; i < nchnls; i++) {
		
		Bone* chnl = anim.Channels[i];
		buffer_size += sizeof(uint); // Ns Size
		buffer_size += sizeof(char) * chnl->name.length() + 1; // N Size
	
		buffer_size += sizeof(chnl->NumPositionKeys()); // nP Size
		buffer_size += sizeof(double) * chnl->NumPositionKeys(); // Pt Size
		buffer_size += sizeof(float) * 3 * chnl->NumPositionKeys(); // Pv Size

		buffer_size += sizeof(chnl->NumRotationKeys()); // nR Size
		buffer_size += sizeof(double) * chnl->NumRotationKeys(); // Rt Size
		buffer_size += sizeof(float) * 4 * chnl->NumRotationKeys(); // Rv Size

		buffer_size += sizeof(chnl->NumScalingKeys()); // nP Size
		buffer_size += sizeof(double) * chnl->NumScalingKeys(); // Pt Size
		buffer_size += sizeof(float) * 3 * chnl->NumScalingKeys(); // Pv Size

	}
	// !_Bone Buffer Sizing
	// !_Buffer Sizing

	// Buffer Alloc
	// Reminder: Buffer Format
	// [[Ns N Dur TpS nCh Chnls[]]

	char* buffer = new char[buffer_size];
	char* it = buffer;

	// Name Size
	memcpy(it, &name_size, sizeof(name_size));
	it += sizeof(name_size);
	// Name
	const char* tmp_name = anim.name.c_str();
	memcpy(it, tmp_name, name_size);
	it += name_size;
	// Duration
	memcpy(it, &anim.duration, dur_size);
	it += dur_size;
	// Ticks per Sec
	memcpy(it, &anim.tickspersec, tps_size);
	it += tps_size;
	// Num Channels
	uint tmp_nChnls = anim.NumChannels();
	memcpy(it, &tmp_nChnls, nchnls_size);
	if(anim.NumChannels() > 0)
		it += nchnls_size;
	// Channels
	for (uint i = 0; i < anim.NumChannels(); i++) {
		// Friendly Reminder: Channel Buffer Format:
		// [Ns N nP Pt Pv nR Rt Rv nS St Sv]
		Bone* chnl = anim.Channels[i];
		// Name Size
		uint tmp_chNs = chnl->name.size();
		memcpy(it, &tmp_chNs, sizeof(uint));
		it += sizeof(uint);
		// Name
		memcpy(it, chnl->name.c_str(), sizeof(char) * tmp_chNs + 1);
		it += sizeof(char) * tmp_chNs + 1;
		// Positions -----------------
		// Num Positions
		int tmp_npk = chnl->NumPositionKeys();
		memcpy(it, &tmp_npk, sizeof(tmp_npk));
		it += sizeof(tmp_npk);
		for (uint n = 0; n < chnl->NumPositionKeys(); n++) {
			TransformKeys::VectorKey* pk = &chnl->transKeys.positionKeys[n];
			// Time
			memcpy(it, &pk->time, sizeof(double));
			it += sizeof(double);
			// Value
			memcpy(it, &pk->value, sizeof(float) * 3);
			it += sizeof(float) * 3;
		}
		// !_Positions ---------------
		// Rotations -----------------
		// Num Rotations
		int tmp_nrk = chnl->NumRotationKeys();
		memcpy(it, &tmp_nrk, sizeof(tmp_nrk));
		it += sizeof(tmp_nrk);
		for (uint n = 0; n < chnl->NumRotationKeys(); n++) {
			TransformKeys::QuatKey* rk = &chnl->transKeys.rotationKeys[n];
			// Time
			memcpy(it, &rk->time, sizeof(double));
			it += sizeof(double);
			// Value
			memcpy(it, &rk->value, sizeof(float) * 4);
			it += sizeof(float) * 4;
		}
		// !_Rotations ---------------
		// Scalings ------------------
		// Num Scalings
		int tmp_nsk = chnl->NumScalingKeys();
		memcpy(it, &tmp_nsk, sizeof(tmp_nsk));
		it += sizeof(tmp_nsk);
		for (uint n = 0; n < chnl->NumScalingKeys(); n++) {
			TransformKeys::VectorKey* sk = &chnl->transKeys.scalingKeys[n];
			// Time
			memcpy(it, &sk->time, sizeof(double));
			it += sizeof(double);
			// Value
			memcpy(it, &sk->value, sizeof(float) * 3);
			it += sizeof(float) * 3;
		}
		// !_Scalings ----------------
	}
	// !_Channels
	// !_Buffer Alloc

	// And FINALLY, we actually save the file... :S
	ret = App->fs->SaveUnique(LIBRARY_ANIMATIONS, buffer, anim.name.c_str(), EXTENSION_ANIMATIONS, buffer_size, output_file);

	if (buffer != nullptr)
		mdelete[] buffer;

	return ret;
}

Bone * ModuleAnimationLoader::ImportBone(const aiNodeAnim * node)
{
	if (node == nullptr)
		return nullptr;

	Bone* new_b = new Bone();

	new_b->name = node->mNodeName.C_Str();
	
	// Translation
	for (int i = 0; i < node->mNumPositionKeys; i++) {
		TransformKeys::VectorKey newPosKey;
		newPosKey.time = node->mPositionKeys[i].mTime;
		newPosKey.value.x = node->mPositionKeys[i].mValue.x;
		newPosKey.value.y = node->mPositionKeys[i].mValue.y;
		newPosKey.value.z = node->mPositionKeys[i].mValue.z;
		new_b->transKeys.positionKeys.push_back(newPosKey);
	}
	// Rotation
	for (int i = 0; i < node->mNumRotationKeys; i++) {
		TransformKeys::QuatKey newRotKey;
		newRotKey.time = node->mRotationKeys[i].mTime;
		newRotKey.value.x = node->mRotationKeys[i].mValue.x;
		newRotKey.value.y = node->mRotationKeys[i].mValue.y;
		newRotKey.value.z = node->mRotationKeys[i].mValue.z;
		newRotKey.value.w = node->mRotationKeys[i].mValue.w;
		new_b->transKeys.rotationKeys.push_back(newRotKey);
	}
	// Scaling
	for (int i = 0; i < node->mNumScalingKeys; i++) {
		TransformKeys::VectorKey newScaleKey;
		newScaleKey.time = node->mScalingKeys[i].mTime;
		newScaleKey.value.x = node->mScalingKeys[i].mValue.x;
		newScaleKey.value.y = node->mScalingKeys[i].mValue.y;
		newScaleKey.value.z = node->mScalingKeys[i].mValue.z;
		new_b->transKeys.scalingKeys.push_back(newScaleKey);
	}

	return new_b;
}