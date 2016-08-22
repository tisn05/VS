namespace Zhangyue.iReader.EBK2
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct ChapterCompress
    {
        public int offset;
        public int length;
    }
}

