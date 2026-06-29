using Data;
namespace DataTest
{
    [TestClass]
    public class DataApiTest
    {
        [TestMethod]
        public void CreateApiAndBoardTest()
        {
            int width = 800;
            int height = 400;

            DataAbstractApi api = DataAbstractApi.CreateApi(width, height);
            IBoard board = api.GetBoard();

            Assert.IsNotNull(api, "API nie powinno być nullem.");
            Assert.IsNotNull(board, "Board nie powinien być nullem.");
            Assert.AreEqual(width, board.width, "Szerokość planszy jest niepoprawna.");
            Assert.AreEqual(height, board.height, "Wysokość planszy jest niepoprawna.");
        }

        [TestMethod]
        public void CreateBallTest()
        {

            double px = 10, py = 20, vx = 2, vy = -2, radius = 15, weight = 20;
            IBall ball = DataAbstractApi.CreateBall(px, py, vx, vy, radius, weight);

            Assert.AreEqual(px, ball.position.X);
            Assert.AreEqual(py, ball.position.Y);
            Assert.AreEqual(vx, ball.velocity.X);
            Assert.AreEqual(vy, ball.velocity.Y);
            Assert.AreEqual(radius, ball.radius);
            Assert.AreEqual(weight, ball.weight);
        }
    }
}
