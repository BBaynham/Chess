#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <string>


namespace Fastsheep {

namespace Bitboards {
void init();
std::string pretty(Bitboard b);
}

using Key = uint64_t;
using Bitboard = uint64_t;

enum Move : int {
	MOVE_NONE,
	MOVE_NULL = 65
};

enum MoveType {
	NORMAL,
	PROMOTION = 1 << 14,
	EN_PASSANT = 2 << 14,
	CASTLING = 3 << 14
};

enum Color {
	WHITE, BLACK, COLOR_NB = 2
};

enum CastlingRights {
	NO_CASTLING,
	WHITE_OO,
	WHITE_OOO = WHITE_OO << 1,
	BLACK_OO = WHITE_OO << 2,
	BLACK_OOO = WHITE_OO << 3,

	KING_SIDE = WHITE_OO | BLACK_OO,
	QUEEN_SIDE = WHITE_OOO | BLACK_OOO,
	WHITE_CASTLING = WHITE_OO | WHITE_OOO,
	BLACK_CASTLING = BLACK_OO | BLACK_OOO,
	ANY_CASTLING = WHITE_CASTLING | BLACK_CASTLING,

	CASTLING_RIGHT_NB = 16
};

enum PieceType { 
	NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
	ALL_PIECES = 0,
	PIECE_TYPE_NB = 8
};

enum Piece {
	NO_PIECE,
	W_PAWN = PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
	B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
	PIECE_NB = 16
};

enum Square : int {
	SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
	SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
	SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
	SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
	SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
	SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
	SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
	SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
	SQ_NONE,

	SQUARE_ZERO = 0,
	SQUARE_NB   = 64
};

enum Direction : int {
	NORTH = 8,
	EAST = 1,
	SOUTH = -NORTH,
	WEST = -EAST,

	NORTH_EAST = NORTH + EAST,
	SOUTH_EAST = SOUTH + EAST,
	SOUTH_WEST = SOUTH + WEST,
	NORTH_WEST = NORTH + WEST
};

enum File : int {
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NB
};

enum Rank : int {
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NB
};

constexpr bool is_ok(Move m) {
  return m != MOVE_NONE && m != MOVE_NULL;
}

constexpr bool is_ok(Square s) {
	return s >= SQ_A1 && S <= SQ_H8;
}

//dirty pieces? types.h and nnue


////Bitboard Setup

constexpr Bitboard FileABB = 0x0101010101010101ULL;
constexpr Bitboard FileBBB = FileABB << 1;
constexpr Bitboard FileCBB = FileABB << 2;
constexpr Bitboard FileDBB = FileABB << 3;
constexpr Bitboard FileEBB = FileABB << 4;
constexpr Bitboard FileFBB = FileABB << 5;
constexpr Bitboard FileGBB = FileABB << 6;
constexpr Bitboard FileHBB = FileABB << 7;

constexpr Bitboard Rank1BB = 0xFF;
constexpr Bitboard Rank2BB = Rank1BB << (8 * 1);
constexpr Bitboard Rank3BB = Rank1BB << (8 * 2);
constexpr Bitboard Rank4BB = Rank1BB << (8 * 3);
constexpr Bitboard Rank5BB = Rank1BB << (8 * 4);
constexpr Bitboard Rank6BB = Rank1BB << (8 * 5);
constexpr Bitboard Rank7BB = Rank1BB << (8 * 6);
constexpr Bitboard Rank8BB = Rank1BB << (8 * 7);

extern uint8_t PopCnt16[1<<16]
extern uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

extern Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
extern Bitboard LineBB[SQUARE_NB][SQUARE_NB];
extern Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
extern Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];


struct Magic {
	Bitboard mask;
	Bitboard magic;
	Bitboard* attacks;
	unsigned shift;

	unsigned index(Bitboard occupied) const { //If statement left over from non 64 bit assembly
		return unsigned(((occupied & mask) * magic) >> shift);
	}
};

extern Magic RookMagics[SQUARE_NB];
extern Magic BishopMagics[SQUARE_NB];

inline Bitboard square_bb(Square s) {
	assert(is_ok(s));
	return (1ULL << s);
}


/// Overloads of bitwise operators between a Bitboard and a Square for testing
/// whether a given bit is set in a bitboard, and for setting and clearing bits.

inline Bitboard  operator&( Bitboard  b, Square s) { return b &  square_bb(s); }
inline Bitboard  operator|( Bitboard  b, Square s) { return b |  square_bb(s); }
inline Bitboard  operator^( Bitboard  b, Square s) { return b ^  square_bb(s); }
inline Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
inline Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

inline Bitboard  operator&(Square s, Bitboard b) { return b & s; }
inline Bitboard  operator|(Square s, Bitboard b) { return b | s; }
inline Bitboard  operator^(Square s, Bitboard b) { return b ^ s; }

inline Bitboard  operator|(Square s1, Square s2) { return square_bb(s1) | s2; }

/// pawn_attacks_bb() returns the squares attacked by pawns of the given color
/// from the squares in the given bitboard.

template<Color C>
constexpr Bitboard pawn_attacks_bb(Bitboard b) {
	return C == WHITE ? shift<NORTH_WEST>(b) | shift<NORTH_EAST>(b)
		: shift<SOUTH_WEST>(b) | shift<SOUTH_EAST>(b);
}

inline Bitboard pawn_attacks_bb(Color c, Square s) {
	assert(is_ok(s));
	return PawnAttacks[c][s];
}

/// distance() functions return the distance between x and y, defined as the
/// number of steps for a king in x to reach y.

template<typename T1 = Square> inline int distance(Square x, Square y);
template<> inline int distance<File>(Square x, Square y) { return std::abs(file_of(x) - file_of(y)); }
template<> inline int distance<Rank>(Square x, Square y) { return std::abs(rank_of(x) - rank_of(y)); }
template<> inline int distance<Square>(Square x, Square y) { return SquareDistance[x][y]; }

template<PieceType Pt>
inline Bitboard attacks_bb(Square s, Bitboard occupied) {
	
	assert((Pt != PAWN) && (is_ok(s)));

	switch (Pt)
	{
	case BISHOP: return BishopMagics[s].attacks[BishopMagics[s].index(occupied)];
	case ROOK  : return   RookMagics[s].attacks[  RookMagics[s].index(occupied)];
	case QUEEN : return attacks_bb<BISHOP>(s, occupied) | attacks_bb<ROOK>(s, occupied);
	default    : return PseudoAttacks[Pt][s];
	}
}

inline Bitboard attacks_bb(PieceType pt, Square s, Bitboard occupied) {
	
	assert((pt != PAWN) && (is_ok(s)));

	switch (pt)
	{
	case BISHOP: return attacks_bb<BISHOP>(s, occupied);
	case ROOK  : return attacks_bb<  ROOK>(s, occupied);
	case QUEEN : return attacks_bb<BISHOP>(s, occupied) | attacks_bb<ROOK>(s, occupied);
	default    : return PseudoAttacks[pt][s];
	}
}

/// popcount() counts the number of non-zero bits in a bitboard

inline int popcount(Bitboard b) {
	return __builtin_popcount11(b);
}

} // namespace Fastsheep

