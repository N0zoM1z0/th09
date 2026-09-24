struct GameManagerPlayfieldView
{
    int IsWithinPlayfield(float x, float y, float width, float height);
};

int GameManagerPlayfieldView::IsWithinPlayfield(
    float x, float y, float width, float height)
{
    if (width / 2.0f + x < -144.0f)
        return 0;
    if (144.0f < x - width / 2.0f)
        return 0;
    if (height / 2.0f + y < 0.0f)
        return 0;
    if (448.0f < y - height / 2.0f)
        return 0;
    return 1;
}
