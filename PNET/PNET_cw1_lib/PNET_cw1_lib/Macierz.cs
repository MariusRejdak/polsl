using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace PNET_cw1_lib
{
    public class Macierz: IEnumerable<double>
    {
        public uint sizeN { get; private set; }
        public uint sizeM { get; private set; }
        private double[,] values;
        
        public Macierz(uint sizeN, uint sizeM)
        {
            this.sizeN = sizeN;
            this.sizeM = sizeM;
            values = new double[sizeN, sizeM];
        }

        public double this[uint n, uint m]
        {
            get
            {
                return values[n, m];
            }
            set
            {
                values[n, m] = value;
            }
        }

        public static Macierz operator +(Macierz a, Macierz b)
        {
            if (a.sizeN != b.sizeN || a.sizeM != b.sizeM)
            {
                throw new ArgumentException("Sizes must match");
            }

            Macierz o = new Macierz(a.sizeN, b.sizeM);

            for (uint n = 0; n < a.sizeN; ++n)
            {
                for (uint m = 0; m < a.sizeM; ++m)
                {
                    o[n, m] = a[n, m] + b[n, m];
                }
            }

            return o;
        }

        public static Macierz operator -(Macierz a, Macierz b)
        {
            if (a.sizeN != b.sizeN || a.sizeM != b.sizeM)
            {
                throw new ArgumentException("Sizes must match");
            }

            Macierz o = new Macierz(a.sizeN, b.sizeM);

            for (uint n = 0; n < a.sizeN; ++n)
            {
                for (uint m = 0; m < a.sizeM; ++m)
                {
                    o[n,m] = a[n,m] - b[n,m];
                }
            }

            return o;
        }

        public static Macierz operator *(Macierz a, Macierz b)
        {
            if (a.sizeM != b.sizeN || a.sizeN != b.sizeM)
            {
                throw new ArgumentException("Wrong sizes");
            }

            Macierz o = new Macierz(a.sizeN, b.sizeM);

            for (uint i = 0; i < a.sizeN; i++)
            {
                for (uint j = 0; j < b.sizeM; j++)
                {
                    o[i,j] = 0;
                    for (uint k = 0; k < a.sizeM; k++)
                    {
                        o[i,j] += a[i,k] * b[k,j];
                    }
                }
            }

            return o;
        }

        public double sumAll(Macierz x)
        {
            double o = 0;
            for (uint n = 0; n < x.sizeN; ++n)
            {
                for (uint m = 0; m < x.sizeM; ++m)
                {
                    o += x[n, m];
                }
            }
            return o;
        }

        public IEnumerator<double> GetEnumerator()
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }

    public class MacierzEnum : IEnumerator
    {
        private Macierz m;
        private bool before = true, after = false;
        private uint positionI = 0;
        private uint positionJ = 0;

        public MacierzEnum(Macierz m)
        {
            this.m = m;
        }

        public object Current
        {
            get
            {
                try
                {
                    return m[positionI, positionJ];
                }
                catch (IndexOutOfRangeException)
                {
                    throw new InvalidOperationException();
                }
            }
        }

        public bool MoveNext()
        {
            if (before)
            {
                positionI = 0;
                positionJ = 0;
                before = false;
                return true;
            }
            if (after)
            {
                return false;
            }

            positionI++;
            if (positionI >= m.sizeN)
            {
                positionJ++;
                positionI = 0;
            }

            if (positionJ >= m.sizeM)
            {
                after = true;
                return false;
            }
            
            return true;
        }

        public void Reset()
        {
            positionI = 0;
            positionJ = 0;
            before = true;
        }
    }

}
