namespace RedTrainer {

	extern uintptr_t moduleBase;
	extern bool isFirstFly;

	void setText(bool bActive);

	unsigned int reverseBytes(unsigned int value);

	void playSound(int soundPtr);

	void setBgm(int bgmPtr);

	void setBgm(char bgmText[]);

	///STATS

	void setInvincibility(bool &Invincibility);

	void setInfinityEnergy(bool &bEnergy);

	void setHealth(int healthValue);

	void setMoney(int moneyValue);

	void setNewItem(char itemId);

	///ITEMS

	void setInfinityAddWeapons(bool &bInfAddWeapon);

	void setBodyShop(int bodyShopId, short bodyShopType);

	void setSwordShop(int swordShopId, short swordShopType);

	void setUniqueShop(int uniqueShopId, short uniqueShopType);

	void setWigShop(int wigShopId, short wigShopType);

	void setLifeFuelShop(int lfShopId, short lfShopType);

	void setSkillsShop(int skillShopId, short skillShopType);

	void getWeaponsCount(char &weaponCount);

	void setAddWeapons(char weaponNum, int weaponValue, char weaponCount);

	///CUSTOMIZATION

	void setPlayerType(int playerTypeId);

	void setPlayerSword(int playerSwordId);

	void setPlayerBody(int playerBodyId);

	void setPlayerHair(int playerHairId);

	void setInvisibility(bool &bInvisible);

	///MOVEMENT

	void setSpeed(float speedValue);

	void setFly();

	void setPlayerAnimation(int animId, int animType, int animIdOld, int animTypeOld, bool isSelectable);

	void setWithoutSword(bool &isActive);

	///MISSION

	void setMission(short missionId, char missionName[]);

	void setDifficulty(char difficultyValue);

	void setNoDamage(bool &bNoDamage);

	void setNoKilled(bool &bNoKilled);

	void setNoAlert(bool &bNoAlert);

	void setBattleTimer(float timerValue);

	void setBattlePoints(int battlePointsValue);

	void setMaxCombo(int maxComboValue);

	void setKills(int killsValue);

	void setZandzutsuKills(int zandzutsuKillsValue);

	///RAIDEN FLAG

	void setFlag(char raidenFlag);

	void setRender(unsigned int renderType);

	///OTHER

	void setSize(float sizeValue, char sizeOffset);

	void setFilter(float filterValue, char filterOffset);

	void setMenuType(char menuType);

	void printMessage(unsigned int messageId, int messageNum, int messagePrint, int messagePosition, char messageChar[]);

	///ENEMY

	void setAllEnemies(bool bEnemyActive, unsigned int enemyId = 0, unsigned int enemyType = 0, unsigned int enemySetType = 0, unsigned int enemyFlag = 0);

	//void spawnEnemy(int enemyId, int setType, int type, int flag);
}