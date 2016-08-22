namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct ChapterHeadModelStruct
    {
        public uint chapter_count;
        public uint chapter_name_header_length;
    }
}

