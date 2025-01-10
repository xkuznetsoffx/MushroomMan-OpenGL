#include "Sound.h"

Sound::Sound(const std::string& filename):
    source(0), buffer(0)
{
    // Создаем уникальный контекст для каждого звука  
    device = alcOpenDevice(nullptr);  
    if (!device) {  
        std::cerr << "Failed to open OpenAL device." << std::endl;  
        return;  
    }  

    context = alcCreateContext(device, nullptr);  
    if (!context) {  
        std::cerr << "Failed to create OpenAL context." << std::endl;  
        alcCloseDevice(device);  
        return;  
    }  

    // Сохраняем текущий активный контекст  
    ALCcontext* currentContext = alcGetCurrentContext();  

    // Делаем наш контекст текущим  
    alcMakeContextCurrent(context);  

    if (!loadWAVFile(filename)) {  
        std::cerr << "ERROR:SOUND()" << std::endl;  
        
        // Восстановим предыдущий контекст  
        alcMakeContextCurrent(currentContext);  
    }  
    else {  
        initSource();  
        
        // Восстановим предыдущий контекст  
        alcMakeContextCurrent(currentContext);  
    }  
}

Sound::~Sound()
{
    // Сохраняем текущий активный контекст  
    ALCcontext* currentContext = alcGetCurrentContext();

    // Делаем наш контекст текущим  
    alcMakeContextCurrent(context);

    if (source) {
        alSourceStop(source);
        alDeleteSources(1, &source);
        source = 0;
    }

    if (buffer) {
        alDeleteBuffers(1, &buffer);
        buffer = 0;
    }

    // Уничтожаем контекст и закрываем устройство  
    alcMakeContextCurrent(currentContext);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void Sound::play()
{
    // Сохраняем текущий активный контекст  
    ALCcontext* currentContext = alcGetCurrentContext();

    // Делаем наш контекст текущим  
    alcMakeContextCurrent(context);

    alSourceStop(source);
    alSourceRewind(source);

    alSourcePlay(source);

    // Восстановим предыдущий контекст  
    alcMakeContextCurrent(currentContext);
}

bool Sound::loadWAVFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open WAV file: " << filename << std::endl;
        return false;
    }

    // Чтение заголовков WAV  
    char riff[4], wave[4];
    int chunkSize, subchunk1Size, sampleRate, byteRate;
    short audioFormat, channels, blockAlign, bitsPerSample;

    file.read(riff, 4);
    file.read(reinterpret_cast<char*>(&chunkSize), 4);
    file.read(wave, 4);

    if (std::string(riff, 4) != "RIFF" || std::string(wave, 4) != "WAVE") {
        std::cerr << "Invalid WAV file format" << std::endl;
        return false;
    }

    // Чтение подзаголовка fmt   
    char fmt[4];
    file.read(fmt, 4);
    file.read(reinterpret_cast<char*>(&subchunk1Size), 4);
    file.read(reinterpret_cast<char*>(&audioFormat), 2);
    file.read(reinterpret_cast<char*>(&channels), 2);
    file.read(reinterpret_cast<char*>(&sampleRate), 4);
    file.read(reinterpret_cast<char*>(&byteRate), 4);
    file.read(reinterpret_cast<char*>(&blockAlign), 2);
    file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

    // Поиск подзаголовка data  
    char dataHeader[4];
    int dataSize;
    while (true) {
        file.read(dataHeader, 4);
        file.read(reinterpret_cast<char*>(&dataSize), 4);

        if (std::string(dataHeader, 4) == "data") {
            break;
        }

        // Пропускаем этот чанк  
        file.seekg(dataSize, std::ios_base::cur);
    }

    //std::cout << "Channels: " << channels
    //    << ", Sample Rate: " << sampleRate
    //    << ", Bits Per Sample: " << bitsPerSample
    //    << ", Data Size: " << dataSize << std::endl;

    std::vector<char> data(dataSize);
    file.read(data.data(), dataSize);

    ALenum format;
    if (channels == 1 && bitsPerSample == 8) format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16) format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8) format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16) format = AL_FORMAT_STEREO16;
    else {
        std::cerr << "Unsupported WAV format" << std::endl;
        return false;
    }

    // Создаём буфер OpenAL  
    alGenBuffers(1, &buffer);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error generating buffer: " << alGetString(error) << std::endl;
        return false;
    }

    alBufferData(buffer, format, data.data(), dataSize, sampleRate);
    error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error filling buffer with data: " << alGetString(error) << std::endl;
        return false;
    }

    return true;
}

void Sound::initSource()
{
    // Создаем источник  
    alGenSources(1, &source);
    ALenum error = alGetError();
    if (error) {
        std::cerr << "Error generating source: " << alGetString(error) << std::endl;
        return;
    }

    alSourcei(source, AL_BUFFER, buffer);
    error = alGetError();
    if (error) {
        std::cerr << "Error attaching buffer to source: " << alGetString(error) << std::endl;
        return;
    }
    alSourcef(source, AL_GAIN, 0.5f);
}
