


enum GameStage{STAGE1, STAGE2, STAGE3, STAGE4};

GameStage getCurrentStage();
void setCurrentStage(GameStage gameStage);

void setup_game();
void loop_game();

void resetDefaultLcdText();