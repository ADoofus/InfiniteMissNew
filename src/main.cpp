
#include "../extern/beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "../extern/beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/include/modloader.hpp"

#include <math.h>

#include <cmath>

using namespace il2cpp_utils;

static const Logger* logger;

bool Enabled = true;
int currentScore = 0;
int ModdedScore = 0;
float currentPercentage = 0;

float Round(float val)
{
    float num = val * 100;
    int num2 = (int)num;
    return num2 / 100;
}
 
std::string Format(int Score)
{
    return to_utf8(csstrtostr(*RunMethod<Il2CppString*>(GetClassFromName("", "ScoreFormatter"), "Format", Score)));
}


int calculateMaxScore(int blockCount) {
    int maxScore;
    if (blockCount < 14) {
        if (blockCount == 1) {
            maxScore = 115;
        } else if (blockCount < 5) {
            maxScore = (blockCount - 1) * 230 + 115;
        } else {
            maxScore = (blockCount - 5) * 460 + 1035;
        }
    } else {
        maxScore = (blockCount - 13) * 920 + 4715;
    }
    return maxScore;
}
float calculatePercentage(int maxScore, int resultScore) {
    float resultPercentage = (float)(100 / (float) maxScore * (float) resultScore);
    float value = (int)(resultPercentage * 100 + .5);
    return (float) value / 100;
}

MAKE_HOOK_OFFSETLESS(Name_ScorePercentage, void, Il2CppObject * self) {
    Name_ScorePercentage(self);

    //Fields
    Il2CppObject * Results = * GetFieldValue(self, "_levelCompletionResults");
    Il2CppObject * rankText = * GetFieldValue(self, "_rankText");
    int maxScore;
    float resultPercentage;
    int resultScore;
    int modifiedScore;
    // Default Rank Text
    std::string rankTextLine1 = to_utf8(csstrtostr( * GetPropertyValue < Il2CppString * > (rankText, "text")));
    std::string rankTextLine2 = "";
    // Colors
    std::string colorPositive = "#00B300";
    std::string colorNegative = "#FF0000";
    //Empty for negatives, "+" for positives
    std::string positiveIndicator = "";
    //State Type
    int EndStateType = * GetFieldValue < int > (Results, "levelEndStateType");
    if (EndStateType == 1) {
        modifiedScore = * GetFieldValue < int > (Results, "modifiedScore");
        maxScore = calculateMaxScore( *
            GetPropertyValue < int > ( *
                GetPropertyValue( *
                    GetFieldValue(self, "_difficultyBeatmap"), "beatmapData"), "notesCount"));
        Il2CppObject * GameplayModifiers = * GetFieldValue(Results, "gameplayModifiers");
        bool noFail = * GetFieldValue < bool > (GameplayModifiers, "_noFail");
        bool noObstacles = * GetFieldValue < bool > (GameplayModifiers, "_noObstacles");
        bool noArrows = * GetFieldValue < bool > (GameplayModifiers, "_noArrows");
        bool noBombs = * GetFieldValue < bool > (GameplayModifiers, "_noBombs");
        if (noFail || noObstacles || noArrows || noBombs) {
            resultScore = modifiedScore;
        } else {
            resultScore = * GetFieldValue < int > (Results, "rawScore");
        }
        resultPercentage = calculatePercentage(maxScore, resultScore);
        SetPropertyValue(rankText, "autoSizeTextContainer", false);
        SetPropertyValue(rankText, "enableWordWrapping", false);
        SetPropertyValue(rankText, "richText", true);
        float percentageDifference = Round(resultPercentage - currentPercentage);
        std::string percentageDifferenceColor;
        //Better or same Score
        if (percentageDifference >= 0) {
            percentageDifferenceColor = colorPositive;
            positiveIndicator = "+";
        }
        //Worse Score
        else {
            percentageDifferenceColor = colorNegative;
            positiveIndicator = "";
        }
        rankTextLine2 = "\n<color=" + percentageDifferenceColor + "><size=40%>" + positiveIndicator + std::to_string(round(percentageDifference*100)/100).substr(0, 5) + "<size=30%>%<size=40%> (" + std::to_string(round(currentPercentage*100)/100).substr(0, 5) + "<size=30%>%<size=40%>)";
        rankTextLine1 = "<line-height=30%><size=60%>" + std::to_string(round(resultPercentage*100)/100).substr(0, 5) + "<size=45%>%";
        int averageCutScore = * GetFieldValue < int > (Results, "averageCutScore");
        RunMethod(rankText, "SetText", createcsstr(rankTextLine1 + rankTextLine2));
        //Start Score Diff
        std::string scoreDifference = "";
        std::string scoreDifferenceColor = "";
        if (currentScore != 0) {
            scoreDifference = std::to_string(modifiedScore - currentScore);
            //Better Score
            if ((modifiedScore - currentScore) > 0) {
                scoreDifferenceColor = colorPositive;
                positiveIndicator = "+";
            }
            //Worse Score
            else if ((modifiedScore - currentScore) < 0) {
                scoreDifferenceColor = colorNegative;
                positiveIndicator = "";
            }
            Il2CppObject* _scoreText = *GetFieldValue(self, "_scoreText");
            SetPropertyValue(_scoreText, "autoSizeTextContainer", false);
            SetPropertyValue(_scoreText, "enableWordWrapping", false);
            SetPropertyValue(_scoreText, "richText", true);
            //Build new ScoreText string
            RunMethod(_scoreText, "SetText", createcsstr("<line-height=30%><size=60%>" + std::to_string(modifiedScore) + "\n<size=40%><color=" + scoreDifferenceColor + "><size=40%>" + positiveIndicator + scoreDifference));
            Il2CppObject* NewHighScore = *GetFieldValue(self, "_newHighScoreText");
            RunMethod(NewHighScore, "SetActive", false);
        }
    }
}   
MAKE_HOOK_OFFSETLESS(StandardLevelDetailViewController_RefreshContent_ScorePercentage, void, Il2CppObject* self) {
    logger->info("ENTERED");
    StandardLevelDetailViewController_RefreshContent_ScorePercentage(self);
    logger->info("Called Original");
    Il2CppObject *Level = *GetFieldValue(self, "_level");
    Il2CppString *LevelID = *GetPropertyValue <Il2CppString*> (Level, "levelID");

    Il2CppObject* SelectedBeatmapDifficulty = *GetFieldValue(self, "_selectedDifficultyBeatmap");
    int Difficulty = *GetPropertyValue<int>(SelectedBeatmapDifficulty, "difficulty");
    Il2CppObject* beatMapData = * GetPropertyValue(SelectedBeatmapDifficulty, "beatmapData");
    Il2CppObject* parentDifficultyBeatmapSet = *GetPropertyValue(SelectedBeatmapDifficulty, "parentDifficultyBeatmapSet");
    Il2CppObject* beatmapCharacteristic = *GetPropertyValue(parentDifficultyBeatmapSet, "beatmapCharacteristic");

    Il2CppObject* PlayerData = *GetFieldValue(self, "_playerData");
    Il2CppObject* playerLevelStatsData = *RunMethod(PlayerData, "GetPlayerLevelStatsData", LevelID, Difficulty, beatmapCharacteristic);
    currentScore = *GetPropertyValue <int>(playerLevelStatsData, "highScore");
    int Notes = *GetPropertyValue <int>(beatMapData, "notesCount");
    currentPercentage = calculatePercentage(calculateMaxScore(Notes), currentScore);
    auto ScoreText = *GetFieldValue(self, "_highScoreText");
    RunMethod(ScoreText, "SetText", createcsstr( std::to_string(currentScore) + " (" + std::to_string(currentPercentage) + "%)" ));
    logger->info("ran code");
}

extern "C" void setup(ModInfo& info) {
    info.id = "ScorePercentage";
    info.version = "1.1.0";
    // Create logger
    static std::unique_ptr<const Logger> ptr(new Logger(info));
    logger = ptr.get();
    logger->info("Completed setup!");
}

extern "C"
void load() {
    INSTALL_HOOK_OFFSETLESS(Name_ScorePercentage, FindMethodUnsafe("", "ResultsViewController", "SetDataToUI", 0));
    INSTALL_HOOK_OFFSETLESS(StandardLevelDetailViewController_RefreshContent_ScorePercentage, FindMethodUnsafe("", "StandardLevelDetailView", "RefreshContent", 0));
}