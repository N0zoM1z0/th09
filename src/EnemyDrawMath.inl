float __stdcall EnemyDrawInterpolateWrappedAngle(
    float angle1, float angle2, float factor)
{
    float directDistance;
    float wrappedDistance;

    if (angle1 < angle2)
    {
        directDistance = angle2 - angle1;
        wrappedDistance = angle1 + 6.2831855f - angle2;
    }
    else
    {
        directDistance = angle1 - angle2;
        wrappedDistance = angle2 + 6.2831855f - angle1;
        angle1 = angle2;
    }

    if (directDistance < wrappedDistance)
    {
    }
    else
    {
        directDistance = wrappedDistance;
    }

    return directDistance * factor + angle1;
}
