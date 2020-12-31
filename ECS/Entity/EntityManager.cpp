#include <Entity.h> 
class EntityManager
{
public:
	EntityManager()
	{
		// Initialize the queue with all possible entity IDs
         lastEntity=0;
	}
    
   // hisa->setId(CreateEntity());
	
    int CreateEntity()
	{
		
    if (mLivingEntityCount < MAX_ENTITIES){
    
            // Take an ID from the front of the queue
            int id = mAvailableEntities.front();
		    mAvailableEntities.pop();
            lastEntity++;
            ++mLivingEntityCount;
           

            return id; 
    }
    return -1;
	}

	void DestroyEntity(int entityId)
	{
    	//	assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Invalidate the destroyed entity's signature
	 if (entityId < MAX_ENTITIES){
        mSignatures[entity].reset();
        mAvailableEntities.push(id);
		// Put the destroyed ID at the back of the queue
	
		--mLivingEntityCount;
     }
    }

	void SetSignature(int entityId, Signature signature)
	{
		 
        if (entityId < MAX_ENTITIES){
		// Put this entity's signature into the array
		mSignatures[entityId] = signature;
        }
	}

	Signature GetSignature(int entityId)
	{ 
        
        if (entityId < MAX_ENTITIES){
		// Get this entity's signature from the array
		return mSignatures[entityId];
        }
	}

private:
	unsigned int lastEntity;
    std::queue<int> mAvailableEntities{};

	// Array of signatures where the index corresponds to the entity ID
	std::array<Signature, MAX_ENTITIES> mSignatures{};

	// Total living entities - used to keep limits on how many exist
	uint32_t mLivingEntityCount{};
};