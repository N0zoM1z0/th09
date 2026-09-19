struct PlayerState4OpsView
{
    int updateState00;

    void SetUpdateState(int state);
};

void PlayerState4OpsView::SetUpdateState(int state)
{
    updateState00 = state;
}
