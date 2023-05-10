namespace RtGui {

	extern bool bFly,
		bTestWindow,
		toSpawn,
		toSetWeaponById;

	extern unsigned int enemyId,
		enemyTypeId,
		enemySetTypeId,
		enemyFlagId,
		previousEnemyId,
		previousTypeId,
		playerAnimationValue;

	extern std::chrono::steady_clock::time_point spawnTimer,
	customizationTimer;

	void mainWindow();

	void statsWindow();

	void itemsWindow();

	void customizationWindow();

	void movementWindow();

	void missionWindow();

	void testWindow();

	void otherWindow();

	void enemyWindow();

	void hideSecondWindow();

	void renderStyle(bool isPush);
}