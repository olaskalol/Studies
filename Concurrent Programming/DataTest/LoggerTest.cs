using Data;

namespace DataTest
{
    internal class LoggerFakeBall : IBall
    {
        public Vector2D position { get; private set; } = new Vector2D(10.5, 20.5);
        public Vector2D velocity { get; private set; } = new Vector2D(1.0, -1.0);
        public double radius { get; } = 15;
        public double weight { get; } = 10;
        public object lockObject { get; } = new object();
        public void SetVelocity(Vector2D newVelocity) => velocity = newVelocity;
        public void SetPosition(Vector2D newPosition) => position = newPosition;
        public event EventHandler BallChanged;

        public void StartMoving(CancellationToken token) { }
    }

    [TestClass]
    public class LoggerTest
    {
        [TestMethod]
        public void LogBallState_Should_Write_To_File_Asynchronously()
        {
            string testFileName = "test_async.log";
            Data.Logger logger = new Data.Logger(testFileName);
            IBall fakeBall = new LoggerFakeBall();

            try
            {
                logger.StartLogging();
                logger.LogBallState(fakeBall);

                Task.Delay(100).Wait();
                logger.StopLogging();
            }
            catch (AggregateException ex) when (ex.InnerException is TaskCanceledException || ex.Message.Contains("A task was canceled"))
            {
            }
            catch (TaskCanceledException)
            {
            }

            Assert.IsTrue(File.Exists(testFileName), "Plik logów nie został utworzony.");
            string[] lines = File.ReadAllLines(testFileName);
            Assert.IsNotEmpty(lines, "Plik logów jest pusty, nic nie zostało zapisane.");
        }

        [TestMethod]
        public void LogBallState_Should_Handle_High_Load_Without_Crashing()
        {
            string testFileName = "test_stress.log";

            Data.Logger logger = new Data.Logger(testFileName);
            IBall fakeBall = new LoggerFakeBall();

            try
            {
                logger.StartLogging();

                for (int i = 0; i < 1000; i++)
                {
                    logger.LogBallState(fakeBall);
                }

                Task.Delay(50).Wait();
                logger.StopLogging();

                Assert.IsTrue(true);
            }
            catch (AggregateException ex) when (ex.InnerException is TaskCanceledException || ex.Message.Contains("A task was canceled"))
            {
                Assert.IsTrue(true);
            }
            catch (TaskCanceledException)
            {
                Assert.IsTrue(true);
            }
            catch (Exception ex)
            {
                Assert.Fail($"Test obciążeniowy zawiódł z nieoczekiwanego powodu: {ex.Message}");
            }
        }
    }
}