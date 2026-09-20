struct CardAttackSpawnPosition
{
    float x;
    float y;
    float z;
};

struct EnemyManagerCardAttackSpawnView
{
    void *SpawnEnemyRecord(
        short subroutine,
        CardAttackSpawnPosition *position,
        int valueA,
        char valueB,
        int valueC,
        int mirrored,
        int runImmediately);

    void *SpawnFromCardAttack(
        int subroutine,
        CardAttackSpawnPosition *position,
        int valueA,
        int valueB,
        int valueC,
        int mirrored);
};

void *EnemyManagerCardAttackSpawnView::SpawnFromCardAttack(
    int subroutine,
    CardAttackSpawnPosition *position,
    int valueA,
    int valueB,
    int valueC,
    int mirrored)
{
    return SpawnEnemyRecord(
        static_cast<short>(subroutine),
        position,
        valueA,
        static_cast<char>(valueB),
        valueC,
        mirrored,
        1);
}
