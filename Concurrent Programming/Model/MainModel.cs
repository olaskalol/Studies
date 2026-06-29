using System;
using Logic;
using Data;
using System.Collections.Generic;
namespace Model
   
{
    public class MainModel
    {
        private readonly LogicAbstractApi logic;
        
        public MainModel(LogicAbstractApi inLogic)
        {
            logic = inLogic;
        }

        public void Start(int ballNumber)
        {
            logic.StartSimulation(ballNumber);
        }

        public void Stop()
        {
            logic.StopSimulation();
        }
        public event EventHandler PositionChanged
        {
            add => logic.PositionChanged += value;
            remove => logic.PositionChanged -= value;
        }
        public IEnumerable<IBall> GetBalls()
        {
            return logic.GetBalls();
        }
    }
}
