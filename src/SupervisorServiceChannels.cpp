extern short __fastcall SampleNetworkInput(int side);

void __fastcall ResetSupervisorServiceChannels()
{
    SampleNetworkInput(0);
    SampleNetworkInput(1);
    SampleNetworkInput(2);
}
