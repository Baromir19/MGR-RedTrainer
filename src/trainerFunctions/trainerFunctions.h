namespace RedTrainer {

	extern uintptr_t moduleBase;
	extern bool isFirstFly;

	void setText(bool bActive);

	void setInvincibility(bool &Invincibility);

	void setInfinityEnergy(bool &bEnergy);

	void setHealth(int healthValue);

	void setMoney(int moneyValue);

	void setPlayerType(int playerTypeId);

	void setPlayerSword(int playerSwordId);

	void setPlayerBody(int playerBodyId);

	void setPlayerHair(int playerHairId);

	void setSpeed(float speedValue);

	void setFly();

	//void spawnEnemy(); //
}