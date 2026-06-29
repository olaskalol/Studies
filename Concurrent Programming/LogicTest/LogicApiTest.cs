using Microsoft.VisualStudio.TestTools.UnitTesting;
using Logic;
using Data;
using System.Linq;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace LogicTest
{
    internal class FakeBoard : IBoard
    {
        public double width => 800;
        public double height => 400;
    }

    internal class FakeDataApi : DataAbstractApi
    {
        private readonly IBoard _board = new FakeBoard();
        public override IBoard GetBoard() => _board;
    }

    [TestClass]
    public class LogicApiTest
    {
        [TestMethod]
        public void StartSimulation_CreatesCorrectNumberOfBalls()
        {
            DataAbstractApi fakeDataApi = new FakeDataApi();
            LogicAbstractApi logicApi = LogicAbstractApi.CreateApi(fakeDataApi);
            int expectedCount = 10;

            logicApi.StartSimulation(expectedCount);
            var balls = logicApi.GetBalls().ToList();

            Assert.AreEqual(expectedCount, balls.Count, "Liczba kul wygenerowanych w symulacji jest niepoprawna.");

            logicApi.StopSimulation();
        }

        [TestMethod]
        public void Balls_AreWithinBoardBoundaries_AfterCreation()
        {
            DataAbstractApi fakeDataApi = new FakeDataApi();
            LogicAbstractApi logicApi = LogicAbstractApi.CreateApi(fakeDataApi);
            IBoard board = fakeDataApi.GetBoard();

            logicApi.StartSimulation(20);
            var balls = logicApi.GetBalls();

            foreach (var ball in balls)
            {
                Assert.IsTrue(ball.position.X >= ball.radius, "Kula poza lewą krawędzią.");
                Assert.IsTrue(ball.position.X <= board.width - ball.radius, "Kula poza prawą krawędzią.");
                Assert.IsTrue(ball.position.Y >= ball.radius, "Kula poza górną krawędzią.");
                Assert.IsTrue(ball.position.Y <= board.height - ball.radius, "Kula poza dolną krawędzią.");
            }

            logicApi.StopSimulation();
        }

        [TestMethod]
        public void StopSimulation_StopsBallMovement()
        {
            DataAbstractApi fakeDataApi = new FakeDataApi();
            LogicAbstractApi logicApi = LogicAbstractApi.CreateApi(fakeDataApi);
            logicApi.StartSimulation(5);

            logicApi.StopSimulation();

            var positionsBefore = logicApi.GetBalls().Select(b => b.position).ToList();

            Task.Delay(50).Wait();

            var positionsAfter = logicApi.GetBalls().Select(b => b.position).ToList();

            for (int i = 0; i < positionsBefore.Count; i++)
            {
                Assert.AreEqual(positionsBefore[i].X, positionsAfter[i].X, "Kula nadal porusza się po zatrzymaniu symulacji.");
                Assert.AreEqual(positionsBefore[i].Y, positionsAfter[i].Y, "Kula nadal porusza się po zatrzymaniu symulacji.");
            }
        }

        [TestMethod]
        public async Task StartSimulation_BallsMoveAsynchronously_OverTime()
        {
            DataAbstractApi fakeDataApi = new FakeDataApi();
            LogicAbstractApi logicApi = LogicAbstractApi.CreateApi(fakeDataApi);

            logicApi.StartSimulation(5);
            var initialPositions = logicApi.GetBalls().Select(b => new Vector2D(b.position.X, b.position.Y)).ToList();
            await Task.Delay(100);
            var currentPositions = logicApi.GetBalls().Select(b => new Vector2D(b.position.X, b.position.Y)).ToList();
            logicApi.StopSimulation();

            bool anyBallMoved = false;
            for (int i = 0; i < initialPositions.Count; i++)
            {
                if (initialPositions[i].X != currentPositions[i].X ||
                    initialPositions[i].Y != currentPositions[i].Y)
                {
                    anyBallMoved = true;
                    break;
                }
            }

            Assert.IsTrue(anyBallMoved, "Kule powinny asynchronicznie zmienić swoją pozycję w tle po upływie czasu.");
        }
    }
}