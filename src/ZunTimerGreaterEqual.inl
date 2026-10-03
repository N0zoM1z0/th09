// Signed current-time comparison over the canonical +8 field.
// One inline definition is shared by ZunTimer consumers for ODR-safe ownership.
// noinline is an empirically validated VC7.1 reconstruction emission choice
// preserving the observed call boundary, not proof of an original annotation.
__declspec(noinline) inline unsigned int ZunTimer::operator>=(int value)
{
    return this->current >= value;
}
