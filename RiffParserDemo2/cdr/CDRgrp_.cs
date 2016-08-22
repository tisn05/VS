using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    /// <summary>
    /// 群组
    /// </summary>
    public class CDRgrp_ : RiffList
    {
        public CDRgrp_(CDRFile p)
            : base(p)
        {
            Parent.GroupCount++;
        }
    }
}
