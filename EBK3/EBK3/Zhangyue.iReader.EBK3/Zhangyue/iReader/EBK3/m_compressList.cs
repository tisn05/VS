namespace Zhangyue.iReader.EBK3
{
    using System;

    public class m_compressList
    {
        public int _lenth;
        public int _offset;

        public m_compressList(int offset, int lenth)
        {
            this._offset = offset;
            this._lenth = lenth;
        }
    }
}

