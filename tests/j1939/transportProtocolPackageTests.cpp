/*******************************************************************************
 * @file	transportProtocolPackageTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "j1939/transportProtocolPackage.h"
}

TEST_GROUP( transportProtocolPackage )
{
	tpPack_t pack;
	uint8_t data[ 9 ] = { 10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u, 90u };

	void setup( void )
	{
		pack = createTransportProtocolPackage( data, 9u );
	}
	void teardown( void )
	{
		destroyTransportProtocolPackage( pack );
	}
};

TEST( transportProtocolPackage, given_a_8_bit_streamlined_data_with_size_biger_than_8_then_a_TP_package_is_created_then_destroyed )
{
	// given

	// when

	// then
}

TEST( transportProtocolPackage, given_null_when_destroying_a_tp_package_then_nothing_happens )
{
	// given

	// when
	destroyTransportProtocolPackage( NULL );

	// then
}

TEST( transportProtocolPackage, given_null_data_when_creating_a_tp_package_then_null_returned )
{
	// given

	// when
	
	// then
	CHECK_TRUE( NULL == createTransportProtocolPackage( NULL, 9u ) );
}

TEST( transportProtocolPackage, given_data_size_less_than_9_when_creating_a_tp_package_then_null_returned )
{
	// given

	// when
	
	// then
	CHECK_TRUE( NULL == createTransportProtocolPackage( data, 8u ) );
}

TEST( transportProtocolPackage, given_null_when_requesting_number_of_packages_then_zero_returned )
{
	// given

	// when
	
	// then
	UNSIGNED_LONGS_EQUAL( 0u, getNumberOfPackages( NULL ) );
}

TEST( transportProtocolPackage, given_valid_data_when_requesting_number_of_packages_then_correct_number_returned )
{
	// given

	// when
	
	// then
	UNSIGNED_LONGS_EQUAL( 2u, getNumberOfPackages( pack ) );
}

TEST( transportProtocolPackage, given_an_input_data_size_bigger_than_255_package_worth_when_creating_a_pack_then_number_of_packages_is_255 )
{
	// given
	uint8_t biggerData[ 255u * 7u + 1u ] = { 255u };

	// when
	tpPack_t maxPack = createTransportProtocolPackage( biggerData, sizeof( biggerData ) );

	// then
	UNSIGNED_LONGS_EQUAL( 255u, getNumberOfPackages( maxPack ) );
	destroyTransportProtocolPackage( maxPack );
}

TEST( transportProtocolPackage, given_an_input_data_with_size_14_when_creating_a_pack_then_number_of_packages_is_2 )
{
	// given
	uint8_t exactData[ 7u * 2u ] = { 255u };

	// when
	tpPack_t twoPack = createTransportProtocolPackage( exactData, sizeof( exactData ) );

	// then
	UNSIGNED_LONGS_EQUAL( 2u, getNumberOfPackages( twoPack ) );
	destroyTransportProtocolPackage( twoPack );
}

TEST( transportProtocolPackage, given_null_input_when_retrieving_a_package_then_null_returned )
{
	// given
	
	// when
	uint8_t* output = getNextTransportProtocolPackage( NULL );

	// then
	CHECK_TRUE( NULL == output );
}

TEST( transportProtocolPackage, given_an_input_that_can_be_fit_in_2_packs_when_retrieving_multiple_packs_then_packs_returned_with_each_call_until_null )
{
	// given
	
	// when
	uint8_t* output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 1u, output[ 0 ] );
	output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 2u, output[ 0 ] );
	output = getNextTransportProtocolPackage( pack );

	// then
	CHECK_TRUE( NULL == output );
}

TEST( transportProtocolPackage, given_an_input_which_will_not_fill_two_packs_when_retrieving_the_last_packet_then_rest_of_data_in_pack_is_255 )
{
	// given
	
	// when
	uint8_t* output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 1u, output[ 0 ] );
	output = getNextTransportProtocolPackage( pack );
	

	// then
	UNSIGNED_LONGS_EQUAL( 2u, output[ 0 ] );
	UNSIGNED_LONGS_EQUAL( 80u, output[ 1 ] );
	UNSIGNED_LONGS_EQUAL( 90u, output[ 2 ] );
	UNSIGNED_LONGS_EQUAL( 255u, output[ 3 ] );
	UNSIGNED_LONGS_EQUAL( 255u, output[ 4 ] );
	UNSIGNED_LONGS_EQUAL( 255u, output[ 5 ] );
	UNSIGNED_LONGS_EQUAL( 255u, output[ 6 ] );
	UNSIGNED_LONGS_EQUAL( 255u, output[ 7 ] );
}

TEST( transportProtocolPackage, given_a_null_pack_when_reseting_pack_index_to_a_value_less_than_the_number_of_packs_then_nothing_happens )
{
	// given
	
	// when
	uint8_t* output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 1u, output[ 0 ] );
	output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 2u, output[ 0 ] );
	resetPackIndexTo( NULL, 1u );
	output = getNextTransportProtocolPackage( pack );
	
	// then
	CHECK_TRUE( NULL == output );
}

TEST( transportProtocolPackage, given_an_index_bigger_than_number_of_packs_when_reseting_pack_index_then_nothing_happens )
{
	// given
	
	// when
	uint8_t* output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 1u, output[ 0 ] );
	output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 2u, output[ 0 ] );
	resetPackIndexTo( pack, 3u );
	output = getNextTransportProtocolPackage( pack );
	
	// then
	CHECK_TRUE( NULL == output );
}

TEST( transportProtocolPackage, given_an_index_less_than_number_of_packs_when_reseting_pack_index_then_next_pack_is_index_plus_one )
{
	// given
	
	// when
	uint8_t* output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 1u, output[ 0 ] );
	output = getNextTransportProtocolPackage( pack );
	UNSIGNED_LONGS_EQUAL( 2u, output[ 0 ] );
	resetPackIndexTo( pack, 0u );
	output = getNextTransportProtocolPackage( pack );
	
	// then
	UNSIGNED_LONGS_EQUAL( 1u, output[ 0 ] );
}

TEST( transportProtocolPackage, given_a_null_input_when_reconstructing_message_data_from_packs_then_nothing_happens )
{
	// given
	uint8_t expectedResult[ 9 ] = { 10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u, 90u };

	// when
	setNextTransportProtocolPackage( pack, NULL );

	// then
	MEMCMP_EQUAL( expectedResult, data, sizeof( data ) );
}

TEST( transportProtocolPackage, given_a_null_pack_and_a_valid_input_when_reconstructing_message_data_from_packs_then_nothing_happens )
{
	// given
	uint8_t expectedResult[ 9 ] = { 10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u, 90u };
	uint8_t inputDataPack[ 8 ] = { 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u };

	// when
	setNextTransportProtocolPackage( NULL, inputDataPack );

	// then
	MEMCMP_EQUAL( expectedResult, data, sizeof( data ) );
}

TEST( transportProtocolPackage, given_an_input_package_with_index_higher_than_transmitted_maximum_number_of_packs_when_reconstructing_message_data_from_packs_then_nothing_happnes )
{
	// given
	uint8_t expectedResult[ 9 ] = { 10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u, 90u };
	uint8_t inputDataPack[ 8 ] = { 3u, 2u, 3u, 4u, 5u, 6u, 7u, 8u };

	// when
	setNextTransportProtocolPackage( pack, inputDataPack );

	// then
	MEMCMP_EQUAL( expectedResult, data, sizeof( data ) );
}

TEST( transportProtocolPackage, given_the_last_received_package_when_reconstructing_message_data_from_packs_then_no_out_of_bound_writes )
{
	// given
	uint8_t expectedResult[ 9 ] = { 10u, 20u, 30u, 40u, 50u, 60u, 70u, 1u, 2u };
	uint8_t inputDataPack[ 8 ] = { 2u, 1u, 2u, 255u, 255u, 255u, 255u, 255u };

	// when
	setNextTransportProtocolPackage( pack, inputDataPack );

	// then
	MEMCMP_EQUAL( expectedResult, data, sizeof( data ) );
}

TEST( transportProtocolPackage, given_multiple_packs_when_reconstructing_data_then_getting_the_packages_will_return_exactly_the_same_data )
{
	// given
	uint8_t inputDataPack1[ 8 ] = { 1u, 1u, 2u, 3u, 4u, 5u, 6u, 7u };
	uint8_t inputDataPack2[ 8 ] = { 2u, 8u, 9u, 255u, 255u, 255u, 255u, 255u };

	// when
	setNextTransportProtocolPackage( pack, inputDataPack1 );
	setNextTransportProtocolPackage( pack, inputDataPack2 );

	// then
	resetPackIndexTo( pack, 0u );
	MEMCMP_EQUAL( inputDataPack1, getNextTransportProtocolPackage( pack ), 8u );
	MEMCMP_EQUAL( inputDataPack2, getNextTransportProtocolPackage( pack ), 8u );
}

TEST( transportProtocolPackage, given_an_in_range_index_when_setting_data_from_a_pack_then_setting_continues_from_that_index )
{
	// given
	uint8_t inputDataPack1[ 8 ] = { 1u, 1u, 2u, 3u, 4u, 5u, 6u, 7u };
	uint8_t inputDataPack2[ 8 ] = { 2u, 8u, 9u, 255u, 255u, 255u, 255u, 255u };
	uint8_t inputDataPack3[ 8 ] = { 2u, 18u, 19u, 255u, 255u, 255u, 255u, 255u };

	// when
	setNextTransportProtocolPackage( pack, inputDataPack1 );
	setNextTransportProtocolPackage( pack, inputDataPack2 );
	resetPackIndexTo( pack, 1u );
	setNextTransportProtocolPackage( pack, inputDataPack3 );


	// then
	resetPackIndexTo( pack, 0u );
	MEMCMP_EQUAL( inputDataPack1, getNextTransportProtocolPackage( pack ), 8u );
	MEMCMP_EQUAL( inputDataPack3, getNextTransportProtocolPackage( pack ), 8u );
}
