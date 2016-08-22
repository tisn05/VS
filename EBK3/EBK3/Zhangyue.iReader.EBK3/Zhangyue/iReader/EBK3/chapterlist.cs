namespace Zhangyue.iReader.EBK3
{
    using System;

    public class chapterlist
    {
        public string _chaptername;
        public int _lenth;
        public int _offset;

        public chapterlist(int offset, int lenth, string chaptername)
        {
            this._offset = offset;
            this._lenth = lenth;
            this._chaptername = chaptername;
        }
    }
}

