namespace RedTrainer {

	extern uintptr_t moduleBase;
	extern bool isFirstFly;

	void setText(bool bActive);

	void setInvincibility(bool &Invincibility);

	void setInfinityEnergy(bool &bEnergy);

	void setHealth(int healthValue);

	void setMoney(int moneyValue);

	void setInfinityAddWeapons(bool &bInfAddWeapon);

	void setBodyShop(int bodyShopId, short bodyShopType);

	void setSwordShop(int swordShopId, short swordShopType);

	void setUniqueShop(int uniqueShopId, short uniqueShopType);

	void setWigShop(int wigShopId, short wigShopType);

	void setLifeFuelShop(int lfShopId, short lfShopType);

	void setSkillsShop(int skillShopId, short skillShopType);

	void setPlayerType(int playerTypeId);

	void setPlayerSword(int playerSwordId);

	void setPlayerBody(int playerBodyId);

	void setPlayerHair(int playerHairId);

	void setSpeed(float speedValue);

	void setFly();

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

	void setFilter(float filterValue, char filterOffset);

	//void spawnEnemy(); //
}