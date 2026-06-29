using System;
using System.Collections.Generic;
using Data;
namespace Logic
{
    public abstract class LogicAbstractApi
    {
        public abstract void StartSimulation(int numberOfBalls);
        public abstract void StopSimulation();
        public abstract IEnumerable<IBall> GetBalls();
        public abstract event EventHandler PositionChanged;

        public static LogicAbstractApi CreateApi(DataAbstractApi dataApi)
        {
            return new LogicAPI(dataApi);
        }
    }
}