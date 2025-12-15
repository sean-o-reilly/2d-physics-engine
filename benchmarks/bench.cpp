#include <benchmark/benchmark.h>
#include <stdexcept>

#include "Environment.h"
#include "Utils.h"

static constexpr float deltaTime = 0.016f;
static constexpr float benchDuration = 10.0f; // approx. time it takes for objects to fall and settle in storm

static void BM_Storm(benchmark::State& state)
{
    Environment env;
    Utils::EnvironmentLoadResult result = Utils::LoadEnvironmentFromJsonFile("saves/environments/storm.json", env);

    if (result != Utils::EnvironmentLoadResult::Success)
    {
        state.SkipWithError("Failed to load environment!");
        return;
    }

    state.counters["DynamicBodies"] = (int)env.GetDynamicBodies().size();
    state.counters["StaticBodies"] = (int)env.GetStaticBodies().size();

    for (auto _ : state)
    {
        env.Update(deltaTime);
    }
}

BENCHMARK(BM_Storm)->Unit(benchmark::kMillisecond)->MinTime(benchDuration);

BENCHMARK_MAIN();