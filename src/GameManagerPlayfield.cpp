struct GameManagerPlayfieldView
{
    int IsWithinPlayfield(float x, float y, float width, float height);
};

int GameManagerPlayfieldView::IsWithinPlayfield(
    float x, float y, float width, float height)
{
    if (width / 2.0f + x < -144.0f)
        return 0;
    if (x - width / 2.0f > 144.0f)
        return 0;
    if (height / 2.0f + y < 0.0f)
        return 0;
    if (y - height / 2.0f > 448.0f)
        return 0;
    return 1;
}
