using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GMS破解
{
    /// <summary>
    /// 二进制数据 操作
    /// </summary>
    public class HexUtility
    {

        public static int Replace(ref Byte[] all, Byte[] s, Byte[] t)
        {
            if (s.Length > all.Length) return 0;
            int replace_count = 0;
            List<byte> temp = new List<byte>();
            for (int i = 0; i < all.Length - s.Length + 1; i++)
            {
                bool catch_s = true;
                for (int j = 0; j < s.Length; j++)
                {
                    if (all[i + j] != s[j])
                    {
                        catch_s = false;
                        break;
                    }
                }
                if (catch_s)
                {
                    replace_count++;
                    temp.AddRange(t);
                    i += s.Length - 1;
                }
                else
                {
                    temp.Add(all[i]);
                }
                if (i == all.Length - s.Length)
                {
                    if (!catch_s)
                    {
                        temp.Add(all[all.Length - 2]);
                        temp.Add(all[all.Length - 1]);
                    }
                }
            }

            all = temp.ToArray();
            return replace_count;
        }

        /// <summary>
        /// 二进制替换,如果没有替换则返回原数组对像的复本.
        /// </summary>
        /// <param name="sourceByteArray">源数据</param>
        /// <param name="oldValue">需要替换的数据</param>
        /// <param name="newValue">将要替换成为的数据</param>
        public static byte[] Replace(byte[] sourceByteArray, byte[] oldValue, byte[] newValue)
        {
            //创建新数据多出1字节
            int newArrayLen = (int)((newValue.Length / (double)oldValue.Length) * sourceByteArray.Length) + 1;
            //得到数组长度
            newArrayLen = Math.Max(newArrayLen, sourceByteArray.Length);
            //新的最后结果数组
            byte[] newByteArray = new byte[newArrayLen];
            //新数组的当前索引
            int curIndex = 0;
            //开始结束
            int start = -1;
            int end = -1;
            //当前查找到的索引
            int oldCurindex = 0;
            //替换数据替换
            for (int x = 0; x < sourceByteArray.Length; x++)
            {
                //查找要替换的数据
                if (sourceByteArray[x] == oldValue[oldCurindex])
                {
                    if (oldCurindex == 0)
                    {
                        start = x;
                    }
                    if (oldCurindex == oldValue.Length - 1)
                    {
                        end = x;
                        oldCurindex = 0;
                    }
                    else
                    {
                        oldCurindex++;
                    }
                }
                else
                {
                    oldCurindex = 0;
                    newByteArray[curIndex] = sourceByteArray[x];
                    curIndex++;
                }
                //数据查找完成
                if (start != -1 && end != -1)
                {
                    //复制替换数据
                    Buffer.BlockCopy(newValue, 0, newByteArray, curIndex, newValue.Length);
                    //计算新数组的偏移量
                    curIndex += newValue.Length;
                    //重新设置需要复制索引的索引
                    start = end = -1;
                }
            }

            //处理返回结果
            byte[] result = null;
            if (curIndex != 0)
            {
                result = new byte[curIndex];
                Buffer.BlockCopy(newByteArray, 0, result, 0, result.Length);
            }
            else
            {
                result = new byte[sourceByteArray.Length];
                Buffer.BlockCopy(sourceByteArray, 0, result, 0, result.Length);
            }
            return result;
        }

        /// <summary>
        /// 二进制替换,如果没有替换则返回原数组对像的复本.
        /// </summary>
        /// <param name="sourceByteArray">源数据</param>
        /// <param name="replaces">需要替换的数据集合</param>
        public static byte[] Replace(byte[] sourceByteArray, List<HexReplaceEntity> replaces)
        {


            //创建新数据多出1字节
            int newArrayLen = (int)((replaces.Sum(p => p.newValue.Length) / (double)replaces.Sum(p => p.oldValue.Length)) * sourceByteArray.Length) + 1;
            //得到数组长度
            newArrayLen = Math.Max(newArrayLen, sourceByteArray.Length);
            //新的最后结果数组
            byte[] newByteArray = new byte[newArrayLen];
            //新数组的当前索引
            int curIndex = 0;
            bool find = false;
            //替换数据替换
            for (int x = 0; x < sourceByteArray.Length; x++)
            {

                foreach (HexReplaceEntity rep in replaces)
                {
                    //查找要替换的数据
                    if (sourceByteArray[x] == rep.oldValue[rep.oldCurindex])
                    {
                        if (rep.oldCurindex == 0)
                        {
                            rep.start = x;
                        }
                        if (rep.oldCurindex == rep.oldValue.Length - 1)
                        {
                            rep.end = x;
                            rep.oldCurindex = 0;
                        }
                        else
                        {
                            rep.oldCurindex++;
                        }
                    }
                    else
                    {
                        rep.oldCurindex = 0;
                        newByteArray[curIndex] = sourceByteArray[x];
                        find = false;
                    }
                    //数据查找完成
                    if (rep.start != -1 && rep.end != -1)
                    {
                        find = true;
                        if (rep.newValue.Length >= rep.oldValue.Length)
                        {
                            //复制替换数据
                            Buffer.BlockCopy(rep.newValue, 0, newByteArray, curIndex, rep.newValue.Length);
                            //计算新数组的偏移量
                            curIndex += rep.newValue.Length;
                        }
                        else
                        //由大字节替换为少字节时出现了问题
                        {
                            curIndex -= rep.end - rep.start;
                            //复制替换数据
                            Buffer.BlockCopy(rep.newValue, 0, newByteArray, curIndex, rep.newValue.Length);
                            //计算新数组的偏移量
                            curIndex += rep.newValue.Length;
                        }
                        //重新设置需要复制索引的索引
                        rep.start = rep.end = -1;
                        break;
                    }
                }
                if (!find)
                {
                    curIndex++;
                }
            }

            //处理返回结果
            byte[] result = null;
            if (curIndex != 0)
            {
                result = new byte[curIndex];
                Buffer.BlockCopy(newByteArray, 0, result, 0, result.Length);
            }
            else
            {
                result = new byte[sourceByteArray.Length];
                Buffer.BlockCopy(sourceByteArray, 0, result, 0, result.Length);
            }
            return result;
        }




    }
    /// <summary>
    /// 替换数据实体
    /// </summary>
    public class HexReplaceEntity
    {
        /// <summary>
        /// 需要替换的原始值
        /// </summary>
        public byte[] oldValue { get; set; }

        /// <summary>
        /// 新值
        /// </summary>
        public byte[] newValue { get; set; }

        /// <summary>
        /// 默认开始结束标记
        /// </summary>
        internal int start = -1;
        /// <summary>
        /// 默认开始结束标记
        /// </summary>
        internal int end = -1;

        //当前查找到的索引
        internal int oldCurindex = 0;

    }
}
