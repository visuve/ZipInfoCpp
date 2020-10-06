#include "PCH.hpp"
#include "StopWatch.hpp"
#include "ZIP.hpp"
#include "IO.hpp"

namespace
{
    bool PrintZipArchiveInfo(std::istream& file)
    {
        Zip::Archive archive;
        file >> archive;

        for (const Zip::LocalFileHeader& fileHeader : archive.LocalFileHeaders)
        {
            const float ratio = float(fileHeader.UncompressedSize) / float(fileHeader.CompressedSize);

            const bool encrypted = fileHeader.Flags.test(0);
            const bool aes256 = fileHeader.CompressionMethod == uint16_t(Zip::FileCompressionMethod::Aes256);
            const auto encryption = encrypted && aes256 ? "AES-256" : encrypted ? "yes" : "no";
            const auto method = aes256 && fileHeader.ExtraField.size() > 9 ?
                static_cast<Zip::FileCompressionMethod>(fileHeader.ExtraField[9]) :
                static_cast<Zip::FileCompressionMethod>(fileHeader.CompressionMethod);

            IO::FmtGuard fmtg;
            std::cout
                << "File header: "
                << "\nName:" << fileHeader.FileName
                << "\nRatio: " << int(ratio * 100) - 100 << '%'
                << "\nEncrypted: " << encryption
                << "\nCompression: " << method
                << "\nCRC-32: 0x" << std::hex << std::setfill('0') << std::setw(8) << std::uppercase << fileHeader.Crc32
                << '\n' << std::endl;
        }

        for (const Zip::CentralDirectoryHeader& centralDirectoryHeader : archive.CentralDirectoryHeaders)
        {
            const float ratio = float(centralDirectoryHeader.UncompressedSize) / float(centralDirectoryHeader.CompressedSize);

            IO::FmtGuard fmtg;
            std::cout
                << "Central directory header:"
                << "\nRatio: " << int(ratio * 100) - 100 << '%'
                << "\nEncrypted: " << (centralDirectoryHeader.Flags.test(0) ? "yes" : "no")
                << "\nCRC-32: 0x" << std::hex << std::setfill('0') << std::setw(8) << std::uppercase << centralDirectoryHeader.Crc32
                << '\n' << std::endl;
        }

        std::cout
            << "Central directory footer:"
            << "\nRecords: " << archive.CentralDirectoryEnd.CentralDirectoryCount
            << "\nSize: " << archive.CentralDirectoryEnd.CentralDirectorySize
            << '\n' << std::endl;

        return file.good();
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2 || !argv[1])
    {
        std::cerr << "Usage: ZipInfoCpp.exe <path1> <path2> <path3> ... <path(n)>" << std::endl;
        return EINVAL;
    }

    try
    {
        for (int i = 1; i < argc; ++i)
        {
            const std::string filePath(argv[i]);
            std::ifstream file(filePath, std::ios_base::binary);

            if (!file)
            {
                std::cerr << "Failed to open: " << '"' << filePath << '"' << std::endl;
                return EIO;
            }

            std::cout << filePath << ":\n" << std::endl;
            file.exceptions(std::istream::failbit | std::istream::badbit);

            StopWatch<MilliSeconds> stopWatch;

            if (!PrintZipArchiveInfo(file))
            {
                std::cerr << "Failed to read ZIP archive information!" << std::endl;
                return EIO;
            }
        }
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "A logic exception occurred: " << e.what() << std::endl;
        std::cerr << "The file does not appear to be a ZIP archive!" << std::endl;
        return EIO;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return EIO;
    }

    return 0;
}