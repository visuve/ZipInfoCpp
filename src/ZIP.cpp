#include "PCH.hpp"
#include "ZIP.hpp"
#include "IO.hpp"

namespace Zip
{
    std::istream& operator >> (std::istream& stream, LocalFileHeader& header)
    {
        IO::Extract(stream, header.Version);
        IO::Extract(stream, header.Flags);
        IO::Extract<uint16_t>(stream, reinterpret_cast<uint16_t&>(header.CompressionMethod));
        IO::Extract(stream, header.LastModifiedTime);
        IO::Extract(stream, header.LastModifiedDate);
        IO::Extract(stream, header.Crc32);
        IO::Extract(stream, header.CompressedSize);
        IO::Extract(stream, header.UncompressedSize);
        const uint16_t fileNameLength = IO::Extract<uint16_t>(stream);
        const uint16_t extraFieldLength = IO::Extract<uint16_t>(stream);

        if (fileNameLength)
        {
            header.FileName.resize(fileNameLength);
            stream.read(&header.FileName.front(), fileNameLength);
        }

        if (extraFieldLength)
        {
            header.ExtraField.resize(extraFieldLength);
            auto extrafieldFront = reinterpret_cast<char*>(&header.ExtraField.front());
            stream.read(extrafieldFront, extraFieldLength);
        }

        return stream;
    }

    std::istream& operator >> (std::istream& stream, CentralDirectoryHeader& header)
    {
        IO::Extract(stream, header.Version);
        IO::Extract(stream, header.VersionRequired);
        IO::Extract(stream, header.Flags);
        IO::Extract(stream, header.CompressionMethod);
        IO::Extract(stream, header.LastModifiedTime);
        IO::Extract(stream, header.LastModifiedDate);
        IO::Extract(stream, header.Crc32);
        IO::Extract(stream, header.CompressedSize);
        IO::Extract(stream, header.UncompressedSize);
        const uint16_t fileNameLength = IO::Extract<uint16_t>(stream);
        const uint16_t extraFieldLength = IO::Extract<uint16_t>(stream);
        const uint16_t commentLength = IO::Extract<uint16_t>(stream);
        IO::Extract(stream, header.FileDiskNumber);
        IO::Extract(stream, header.InternalFileAttributes);
        IO::Extract(stream, header.ExternalFileAttributes);
        IO::Extract(stream, header.RelativeOffset);

        if (fileNameLength)
        {
            header.FileName.resize(fileNameLength);
            stream.read(&header.FileName.front(), fileNameLength);
        }

        if (extraFieldLength)
        {
            header.ExtraField.resize(extraFieldLength);
            auto extrafieldFront = reinterpret_cast<char*>(&header.ExtraField.front());
            stream.read(extrafieldFront, extraFieldLength);
        }

        if (commentLength)
        {
            header.Comment.resize(commentLength);
            stream.read(&header.Comment.front(), commentLength);
        }

        return stream;
    }

    std::istream& operator >> (std::istream& stream, EndOfCentralDirectoryRecord& record)
    {
        IO::Extract(stream, record.CurrentDisk);
        IO::Extract(stream, record.CentralDirectoryDisk);
        IO::Extract(stream, record.CurrentDiskCentralDirectoryCount);
        IO::Extract(stream, record.CentralDirectoryCount);
        IO::Extract(stream, record.CentralDirectorySize);
        IO::Extract(stream, record.CentralDirectoryOffset);

        const uint16_t commentLength = IO::Extract<uint16_t>(stream);

        if (commentLength)
        {
            record.Comment.resize(commentLength);
            stream.read(&record.Comment.front(), commentLength);
        }

        return stream;
    }

    std::istream& operator >> (std::istream& stream, Archive& archive)
    {
        bool complete = false;

        while (!complete)
        {
            const uint32_t id = IO::Extract<uint32_t>(stream);

            switch (static_cast<ArchiveMarkers>(id))
            {
            case Zip::ArchiveMarkers::LocalFileHeader:
            {
                LocalFileHeader& header = archive.LocalFileHeaders.emplace_back();
                stream >> header;
                stream.seekg(header.CompressedSize, std::ios_base::cur); // Skip the compressed data
                continue;
            }
            case Zip::ArchiveMarkers::DataDescriptor:
            {
                // TODO: offset might be 8!
                stream.seekg(12, std::ios_base::cur);
                continue;
            }
            case Zip::ArchiveMarkers::CentralDirectoryFileHeader:
            {
                stream >> archive.CentralDirectoryHeaders.emplace_back();
                continue;
            }
            case Zip::ArchiveMarkers::EndOfCentralDirectoryRecord:
            {
                stream >> archive.CentralDirectoryEnd;
                complete = true;
                break;
            }
            case Zip::ArchiveMarkers::TemporarySpanning:
            {
                // TODO: find out more about this
                continue;
            }
            default:
                throw std::logic_error("Read unknown marker: " + std::to_string(id));
            }
        }

        return stream;
    }

    std::ostream& operator << (std::ostream& stream, const FileCompressionMethod method)
    {
        switch (method)
        {
        case FileCompressionMethod::Store:
            return stream << "No compression";
        case FileCompressionMethod::Deflate:
            return stream << "DEFLATE";
        case FileCompressionMethod::Deflate64:
            return stream << "DEFLATE64";
        case FileCompressionMethod::Bzip2:
            return stream << "bzip2";
        case FileCompressionMethod::Lzma:
            return stream << "LZMA";
        case FileCompressionMethod::Ppmd:
            return stream << "PPmD";
        case FileCompressionMethod::Aes256:
            return stream; // See FileCompressionMethod notes
        }

        return stream << "Unknown " << '(' << uint32_t(method) << ')';
    }
}