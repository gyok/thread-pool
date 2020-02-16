#pragma once

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include "../src/threadPool.h"

using namespace testing;

TEST(ThreadPoolUT, ThreadPoolUT)
{
  ThreadPool tp(8);
  auto expected = 8;
}
