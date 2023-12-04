#ifndef VDSPROJECT_UNIQUE_TABLE_TESTS_H
#define VDSPROJECT_UNIQUE_TABLE_TESTS_H

#include <gtest/gtest.h>
#include "../UniqueTable.h"
#include <vector>
#include <string>

class UniqueTableTest : public testing::Test {
protected:
    ClassProject::UniqueTable uniqueTable;
};

TEST_F(UniqueTableTest, InsertAndFindWorks)
{
    ClassProject::Node firstNode{3, 0, 0, 0, "n3"};
    ClassProject::Node secondNode{284, 1, 1, 1, "n284"};
    uniqueTable.insert(firstNode);
    uniqueTable.insert(secondNode);

    EXPECT_EQ(uniqueTable.findById(3)->id, 3);
    EXPECT_EQ(uniqueTable.findById(284)->id, 284);

    EXPECT_EQ(uniqueTable.findByTriple(ClassProject::NodeTriple{0, 0, 0})->id, 3);
    EXPECT_EQ(uniqueTable.findByTriple(ClassProject::NodeTriple{1, 1, 1})->id, 284);
}

TEST_F(UniqueTableTest, SizeWorks)
{
    ClassProject::Node nodeFalse{0, 0, 0, 0, "False"};
    uniqueTable.insert(nodeFalse);
    EXPECT_EQ(uniqueTable.size(), 1);

    ClassProject::Node nodeTrue{1, 1, 1, 1, "True"};
    uniqueTable.insert(nodeTrue);
    EXPECT_EQ(uniqueTable.size(), 2);
}

TEST_F(UniqueTableTest, IteratorsWork)
{
    ClassProject::Node nodeFalse{0, 0, 0, 0, "False"};
    ClassProject::Node nodeTrue{1, 1, 1, 1, "True"};
    ClassProject::Node aNode{2, 2, 1, 0, "a"};
    uniqueTable.insert(nodeFalse);
    uniqueTable.insert(nodeTrue);
    uniqueTable.insert(aNode);

    std::vector<std::string> labels = {"False", "True", "a"};
    unsigned int i = 0;
    for (auto it = uniqueTable.begin(); it != uniqueTable.end(); it++)
    {
        EXPECT_EQ(it->label, labels[i]);
        i++;
    }

    i = 0;
    for (auto it = uniqueTable.cbegin(); it != uniqueTable.cend(); it++)
    {
        EXPECT_EQ(it->label, labels[i]);
        i++;
    }
}

#endif