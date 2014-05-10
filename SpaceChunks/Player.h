#include "XyEngine.h"

class Player
{
public:
	Player(glm::vec3 pos);
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPos();
	void Update(bool mousein);
	void Init();
	~Player();
private:
	glm::vec3 m_pos;
};



