#pragma once

namespace Zip
{
    // See following links for more details:
    // https://en.wikipedia.org/wiki/Zip_(file_format)
    // https://upload.wikimedia.org/wikipedia/commons/6/63/ZIP-64_Internal_Layout.svg
    // https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html

     // Markers to divide sections in a ZIP archive
    enum class ArchiveMarkers : uint32_t
    {
        LocalFileHeader = 0x04034B50, // Local file header signature
        DataDescriptor = 0x08074B50, // Optional data descriptor signature
        CentralDirectoryFileHeader = 0x02014B50, // Central directory file header signature
        EndOfCentralDirectoryRecord = 0x06054B50, // End of central directory signature
        TemporarySpanning = 0x30304B50 // Temporary spanning / split file signature
    };

    // NOTE: this applies to files only, not central directory
    enum class FileCompressionMethod : uint16_t
    {
        Store = 0, // No compression
        Deflate = 8, // Most common
        Deflate64 = 9,
        Bzip2 = 12,
        Lzma = 14,
        Ppmd = 98,
        Aes256 = 99 // With AES-256 encryption the compression method is in ExtraField[9]
    };

    // Describes a file within a ZIP archive
    struct LocalFileHeader
    {
        uint16_t Version = 0; // Version needed to extract (minimum)
        std::bitset<16> Flags; // General purpose bit flag
        uint16_t CompressionMethod = 0; // Compression method
        uint16_t LastModifiedTime = 0; // File last modification time
        uint16_t LastModifiedDate = 0; // File last modification date
        uint32_t Crc32 = 0; // CRC-32
        uint32_t CompressedSize = 0; // Compressed size
        uint32_t UncompressedSize = 0; // Uncompressed size
        // FileNameLength, included in FileName
        // ExtraFieldLength, included in ExtraField
        std::string FileName; // File name
        std::vector<uint8_t> ExtraField; // Extra field
    };

    // Describes the central directory within a ZIP archive
    struct CentralDirectoryHeader
    {
        uint16_t Version = 0; // Version made by
        uint16_t VersionRequired = 0; // Version needed to extract (minimum)
        std::bitset<16> Flags; // General purpose bit flag
        uint16_t CompressionMethod = 0; // Compression method
        uint16_t LastModifiedTime = 0; //  	File last modification time
        uint16_t LastModifiedDate = 0; // File last modification date
        uint32_t Crc32 = 0; // CRC-32
        uint32_t CompressedSize = 0; // Compressed size
        uint32_t UncompressedSize = 0; // Uncompressed size
        // FileNameLength, included in FileName
        // ExtraFieldLength, included in ExtraField
        // CommentLength, included in Comment
        uint16_t FileDiskNumber = 0; // Disk number where file starts
        uint16_t InternalFileAttributes = 0; // Internal file attributes
        uint32_t ExternalFileAttributes = 0; // External file attributes
        uint32_t RelativeOffset = 0; // Relative offset of local file header
        std::string FileName; // File name
        std::vector<uint8_t> ExtraField; // Extra field
        std::string Comment; // File comment
    };

    struct EndOfCentralDirectoryRecord
    {
        uint16_t CurrentDisk = 0; // Number of this disk
        uint16_t CentralDirectoryDisk = 0; // Disk where central directory starts
        uint16_t CurrentDiskCentralDirectoryCount = 0; // Number of central directory records on this disk
        uint16_t CentralDirectoryCount = 0; // Total number of central directory records
        uint32_t CentralDirectorySize = 0; // Size of central directory (bytes)
        uint32_t CentralDirectoryOffset = 0; // Offset of start of central directory, relative to start of archive
        // CommentLength, included in Comment
        std::string Comment;
    };

    struct Archive
    {
        std::vector<LocalFileHeader> LocalFileHeaders;
        std::vector<CentralDirectoryHeader> CentralDirectoryHeaders;
        EndOfCentralDirectoryRecord CentralDirectoryEnd;
    };

    std::istream& operator >> (std::istream&, LocalFileHeader&);
    std::istream& operator >> (std::istream&, CentralDirectoryHeader&);
    std::istream& operator >> (std::istream&, EndOfCentralDirectoryRecord&);
    std::istream& operator >> (std::istream&, Archive&);
    std::ostream& operator << (std::ostream&, const FileCompressionMethod);
}