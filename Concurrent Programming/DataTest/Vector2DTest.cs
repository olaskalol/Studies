using Data;

namespace DataTest
{
    [TestClass]
    public class Vector2DTest
    {
        [TestMethod]
        public void VectorAdditionTest()
        {
            Vector2D v1 = new Vector2D(10, 20);
            Vector2D v2 = new Vector2D(5, -5);

            Vector2D result = new Vector2D(v1.X + v2.X, v1.Y + v2.Y);

            Assert.AreEqual(15, result.X, "Suma składowych X jest niepoprawna.");
            Assert.AreEqual(15, result.Y, "Suma składowych Y jest niepoprawna.");
        }

        [TestMethod]
        public void VectorConstructorTest()
        {
            Vector2D v = new Vector2D(1.23, 4.56);

            Assert.AreEqual(1.23, v.X);
            Assert.AreEqual(4.56, v.Y);
        }
    }
}