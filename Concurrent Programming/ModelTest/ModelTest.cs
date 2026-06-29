using Model;
using Logic;
using Data;

namespace ModelTest
{
    
    internal class FakeLogic : LogicAbstractApi 
    {
        public int StartSimulationCalledCount = 0;
        public int StopSimulationCalledCount = 0;
        public int LastRequestedBallCount = 0;
        public override event EventHandler PositionChanged;

        public override void StartSimulation(int numberOfBalls)
        {
            StartSimulationCalledCount++;
            LastRequestedBallCount = numberOfBalls;
        }

        public override void StopSimulation()
        {
            StopSimulationCalledCount++;
        }

        public override IEnumerable<IBall> GetBalls()
        {
            return new List<IBall>();
        }

        
    }

    [TestClass]
    public sealed class ModelTests
    {
        [TestMethod]
        public void StartSimulationTest()
        {
            FakeLogic fl = new FakeLogic();
            MainModel model = new MainModel(fl);
            int balls = 7;
            model.Start(balls);

            Assert.AreEqual(1, fl.StartSimulationCalledCount, "Metoda StartSimulation z warstwy logiki powinna zostać wywołana dokładnie raz.");
            Assert.AreEqual(balls, fl.LastRequestedBallCount, "Model powinien przekazać poprawną liczbę kul do warstwy logiki.");
        }

        [TestMethod]
        public void StopSimulationTest()
        {
            FakeLogic fl = new FakeLogic();
            MainModel model = new MainModel(fl);
            model.Stop();
            Assert.AreEqual(1, fl.StopSimulationCalledCount, "Metoda StopSimulation z logiki powinna zostać wywołana.");
        }

        [TestMethod]
        public void ReturnDataFromLogicTest()
        {
            FakeLogic fl = new FakeLogic();
            MainModel model = new MainModel(fl);
            var balls = model.GetBalls();
            Assert.IsNotNull(balls, "Zwrócona kolekcja kul nie powinna być null.");
        }
    }
}
