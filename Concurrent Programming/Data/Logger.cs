using System;
using System.Collections.Concurrent;
using System.IO;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Threading.Tasks;

[assembly: InternalsVisibleTo("DataTest")]

namespace Data
{
   internal class Logger
    {
        private readonly string filePath;
        private readonly ConcurrentQueue<string> logQueue = new ConcurrentQueue<string>();
        private CancellationTokenSource cts;
        private Task loggingTask;

        public Logger(string path = "diagnostics.log")
        {
            filePath = path;
            File.WriteAllText(filePath, string.Empty);
        }


        public void StartLogging()
        {
            cts = new CancellationTokenSource();
            loggingTask = Task.Run(async () => await ProcessQueue(cts.Token));
        }

        public void StopLogging()
        {
            cts?.Cancel();
            loggingTask?.Wait();
        }

        public void LogBallState(IBall ball)
        {
            string logEntry = $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}] Ball_{ball.GetHashCode()} | Pos: ({ball.position.X:F2}, {ball.position.Y:F2}) | Vel: ({ball.velocity.X:F2}, {ball.velocity.Y:F2})";
            logQueue.Enqueue(logEntry);
        }

        private async Task ProcessQueue(CancellationToken token)
        {
            try
            {
                using StreamWriter writer = new StreamWriter(filePath, append: true, encoding: System.Text.Encoding.ASCII);
                writer.AutoFlush = true;

                while (!token.IsCancellationRequested)
                {
                    if (logQueue.TryDequeue(out string logEntry))
                    {
                        await writer.WriteLineAsync(logEntry);
                    }
                    else
                    {
                        await Task.Delay(10, token);
                    }
                }
            }
            catch (Exception)
            {
                await Task.Delay(50, token);
            }
        }
    }
}