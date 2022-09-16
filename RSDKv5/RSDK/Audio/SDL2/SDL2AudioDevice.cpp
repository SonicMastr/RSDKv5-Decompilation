
uint8 AudioDevice::contextInitialized;

SDL_AudioDeviceID AudioDevice::device;
SDL_AudioSpec AudioDevice::deviceSpec;

bool32 AudioDevice::Init()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (!contextInitialized) {
        contextInitialized = true;
        InitAudioChannels();
    }

    SDL_AudioSpec want;
    want.freq     = AUDIO_FREQUENCY;
    want.format   = AUDIO_F32SYS;
    want.samples  = MIX_BUFFER_SIZE / AUDIO_CHANNELS;
    want.channels = AUDIO_CHANNELS;
    want.callback = AudioCallback;

    audioState = false;
    if ((device = SDL_OpenAudioDevice(nullptr, 0, &want, &deviceSpec, 0)) > 0) {
        SDL_PauseAudioDevice(device, SDL_FALSE);
        audioState = true;
    }
    else {
        PrintLog(PRINT_NORMAL, "ERROR: Unable to open audio device!");
        PrintLog(PRINT_NORMAL, "ERROR: %s", SDL_GetError());
    }

    return true;
}

void AudioDevice::Release()
{
    LockAudioDevice();

    if (vorbisInfo) {
        vorbis_deinit(vorbisInfo);
        if (!vorbisInfo->alloc.alloc_buffer)
            free(vorbisInfo);
    }
    vorbisInfo = NULL;

    UnlockAudioDevice();

    SDL_CloseAudioDevice(AudioDevice::device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void AudioDevice::InitAudioChannels()
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        channels[i].soundID = -1;
        channels[i].state   = CHANNEL_IDLE;
    }

    for (int32 i = 0; i < 0x400; i += 2) {
        speedMixAmounts[i]     = (i + 0) * (1.0f / 1024.0f);
        speedMixAmounts[i + 1] = (i + 1) * (1.0f / 1024.0f);
    }

    GEN_HASH_MD5("Stream Channel 0", sfxList[SFX_COUNT - 1].hash);
    sfxList[SFX_COUNT - 1].scope              = SCOPE_GLOBAL;
    sfxList[SFX_COUNT - 1].maxConcurrentPlays = 1;
    sfxList[SFX_COUNT - 1].length             = MIX_BUFFER_SIZE;
    AllocateStorage((void **)&sfxList[SFX_COUNT - 1].buffer, MIX_BUFFER_SIZE * sizeof(SAMPLE_FORMAT), DATASET_MUS, false);

    initializedAudioChannels = true;
}

void AudioDevice::AudioCallback(void *data, uint8 *stream, int32 len)
{
    (void)data; // Unused

    AudioDevice::ProcessAudioMixing(stream, len / sizeof(SAMPLE_FORMAT));
}
