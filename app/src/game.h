


enum GameStage{STAGE1, STAGE2, STAGE3};

static char STAGE1_LCD_TEXT[] = "Stage 1";
static char STAGE2_LCD_TEXT[] = "Stage 2";

GameStage getCurrentStage();
void setCurrentStage(GameStage gameStage);

void setup_game();
void loop_game();