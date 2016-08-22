namespace Zhangyue.iReader.EBK2
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct ChapterStruct
    {
        public byte[] chapter_name;
        public int offset;
        public int lenth;
    }
}

